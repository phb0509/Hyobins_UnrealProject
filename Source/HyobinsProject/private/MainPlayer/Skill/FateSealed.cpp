// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/FateSealed.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/Monster.h"
#include "SubSystems/BattleManager.h"
#include "Utility/EnumTypes.h"

UFateSealed::UFateSealed() :
	m_FateSealedMontage(nullptr),
	m_SkillRange(1000.0f),
	m_PPMaterial(nullptr),
	m_FinishSound(nullptr),
	m_CurTargetIndex(0),
	m_FinishAttackDelay(3.0f),
	m_AttackDelay(0.1f)
{
	
}

void UFateSealed::Initialize()
{
	Super::Initialize();
	
	m_BattleManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UBattleManager>();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("FateSealed"),
[this]()
	{
		onFateSealedEnded();
	});

	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("FateSealed_Finish"),
[this]()
	{
		onFateSealedFinishEnded();
	});

	m_DynamicPPMaterial = UMaterialInstanceDynamic::Create(m_PPMaterial, this);
}

void UFateSealed::Execute()
{
	Super::Execute();

	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::FateSealed);
	
	APostProcessVolume* postProcessVolume = m_Owner->GetGlobalPostProcessVolume();
	postProcessVolume->bUnbound = true;
	postProcessVolume->BlendWeight = 1.0f;
	postProcessVolume->Settings.AddBlendable(m_PPMaterial.Get(), 1);
	
	findTargets();
}

void UFateSealed::findTargets()
{
	m_SkillStartLocation = m_Owner->GetActorLocation();
	
	TArray<FHitResult> hitResults;
	FCollisionShape sphereShape = FCollisionShape::MakeSphere(m_SkillRange);
	FCollisionQueryParams params;
	FCollisionObjectQueryParams objectParams = m_BattleManager->MakeCollisionObjectParams(m_Owner.Get());
	
	bool bHit = m_Owner->GetWorld()->SweepMultiByObjectType(
		hitResults,
		m_SkillStartLocation,
		m_SkillStartLocation,
		FQuat::Identity,
		objectParams, 
		sphereShape,
		params
	);

	m_TargetMonsters.Empty();
	
	for (FHitResult& hitResult : hitResults)
	{
		AMonster* targetMonster = Cast<AMonster>(hitResult.GetActor());
		targetMonster->Pause();
		targetMonster->GetMesh()->SetCustomDepthStencilValue(2);
		
		m_TargetMonsters.Add(targetMonster);
	}
	
	m_CurTargetIndex = 0;
	attackNextTarget();
}

void UFateSealed::attackNextTarget()
{
	if (m_CurTargetIndex >= m_TargetMonsters.Num())
	{
		finishSkill();
		
		return;
	}
	
	moveToTarget();
}

void UFateSealed::moveToTarget()
{
	m_CurTarget = m_TargetMonsters[m_CurTargetIndex].Get();
	const FVector targetLocation = m_CurTarget->GetActorLocation();
	const FVector finalLocation = targetLocation + (m_Owner->GetActorLocation() - targetLocation).GetSafeNormal() * 30.0f;

	m_Owner->SetActorLocation(finalLocation);
	m_Owner->RotateToTarget(m_CurTarget.Get());

	const FName section = *FString::FromInt(m_CurTargetIndex % 5);
	m_OwnerAnimInstance->Montage_Play(m_FateSealedMontage,1.0f);
	m_OwnerAnimInstance->Montage_JumpToSection(section, m_FateSealedMontage);
}

void UFateSealed::onFateSealedEnded()
{
	FTimerHandle stencilTimer;
	m_Owner->GetWorldTimerManager().SetTimer(
	stencilTimer,
	[=]() mutable 
	{
		int32 stencil = m_CurTarget->GetMesh()->CustomDepthStencilValue + 1;
		m_CurTarget->GetMesh()->SetCustomDepthStencilValue(stencil);
	
		if (stencil >= 102)
		{
			m_Owner->GetWorldTimerManager().ClearTimer(stencilTimer);
		}
	},
	0.04f,
	true
	);

	++m_CurTargetIndex;
	attackNextTarget();
}

void UFateSealed::finishSkill()
{
	// 葛电 鸥百 贸府 肯丰
	APostProcessVolume* postProcessVolume = m_Owner->GetGlobalPostProcessVolume();
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
		
	postProcessVolume->bUnbound = false;
	postProcessVolume->Settings.RemoveBlendable(m_PPMaterial.Get());
	
	ownerSkillComponent->SetCanChargingSkill(false);
	m_Owner->SetActorLocation(m_SkillStartLocation);
	
	m_OwnerAnimInstance->Montage_Play(m_FateSealedFinishMontage,1.0f);
	
}

void UFateSealed::onFateSealedFinishEnded()
{
	FTimerHandle finishTimer;
	m_Owner->GetWorldTimerManager().SetTimer
		(
			finishTimer,
			[=]() mutable
			{
				float attackTime = 0.1f;
				constexpr float attackGap = 0.1f;
				
				for (TWeakObjectPtr<AMonster> monster : m_TargetMonsters)
				{
					monster->GetMesh()->SetCustomDepthStencilValue(2);

					FTimerHandle attackTimer;
					m_Owner->GetWorldTimerManager().SetTimer
					(
						attackTimer,
						[=]()
						{
							monster->Unpause();
							m_BattleManager->Attack(m_Owner.Get(), TEXT("FateSealed_Finish"), monster.Get());
							
							if (m_FinishSound != nullptr)
							{
								UGameplayStatics::PlaySound2D(m_Owner.Get(), m_FinishSound, 2.0f);
							}
						},
						attackTime += attackGap,
						false
					);
				}
				
				m_TargetMonsters.Empty();
			},
		m_FinishAttackDelay,
		false
		);

	m_Owner->RemoveInputMappingContext(TEXT("Charging_OnGround"));
	m_Owner->AddInputMappingContext(TEXT("Default_OnGround")); 
	m_Owner->SetIsSuperArmor(false);
}


bool UFateSealed::CanExecuteSkill() const
{
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	return Super::CanExecuteSkill() && !m_Owner->IsCrowdControlState() &&
		ownerSkillComponent->CanChargingSkill();
}
