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
	m_PostProcessingMaterial(nullptr),
	m_CurTargetIndex(0),
	m_FinishSound(nullptr)
{
	
}

void UFateSealed::Initialize()
{
	Super::Initialize();

	APostProcessVolume* postProcessVolume = m_Owner->GetGlobalPostProcessVolume();
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	m_BattleManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UBattleManager>();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("FateSealed"),
[=]()
	{
		OnAttackMontageEnded();
	});
}

void UFateSealed::Execute()
{
	Super::Execute();

	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::FateSealed);
	
	APostProcessVolume* postProcessVolume = m_Owner->GetGlobalPostProcessVolume();
	postProcessVolume->bUnbound = true;
	postProcessVolume->BlendWeight = 1.0f;
	postProcessVolume->Settings.AddBlendable(m_PostProcessingMaterial.Get(), 1.0f);
	
	findTargets();
}

void UFateSealed::findTargets()
{
	const FVector startLocation = m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation();
	
	TArray<FHitResult> hitResults;
	FCollisionShape sphereShape = FCollisionShape::MakeSphere(m_SkillRange);
	FCollisionQueryParams params;
	FCollisionObjectQueryParams objectParams = m_BattleManager->MakeCollisionObjectParams(m_Owner.Get());
	
	bool bHit = m_Owner->GetWorld()->SweepMultiByObjectType(
		hitResults,
		startLocation,
		startLocation,
		FQuat::Identity,
		objectParams, 
		sphereShape,
		params
	);

	for (FHitResult& hitResult : hitResults)
	{
		AMonster* targetMonster = Cast<AMonster>(hitResult.GetActor());
		targetMonster->Pause();
		
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
	AMonster* target = m_TargetMonsters[m_CurTargetIndex].Get();
	const FVector targetLocation = target->GetActorLocation();
	const FVector finalLocation = targetLocation + (m_Owner->GetActorLocation() - targetLocation).GetSafeNormal() * 100.0f;

	m_Owner->SetActorLocation(finalLocation);

	const FName section = *FString::FromInt(m_CurTargetIndex % 5);
	m_OwnerAnimInstance->Montage_Play(m_FateSealedMontage,1.0f);
	m_OwnerAnimInstance->Montage_JumpToSection(section, m_FateSealedMontage);
	target->GetMesh()->SetCustomDepthStencilValue(3.0f);
}

void UFateSealed::OnAttackMontageEnded()
{
	++m_CurTargetIndex;
	attackNextTarget();
}

void UFateSealed::finishSkill()
{
	// 葛电 鸥百 贸府 肯丰
	APostProcessVolume* postProcessVolume = m_Owner->GetGlobalPostProcessVolume();
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
		
	postProcessVolume->bUnbound = false;
	postProcessVolume->Settings.RemoveBlendable(m_PostProcessingMaterial.Get());
		
	ownerSkillComponent->SetCanChargingSkill(false);
	m_Owner->RemoveInputMappingContext(TEXT("Charging_OnGround"));
	m_Owner->AddInputMappingContext(TEXT("Default_OnGround")); 
	m_Owner->SetIsSuperArmor(false);

	FTimerHandle finishTimer;
	m_Owner->GetWorldTimerManager().SetTimer
		(
			finishTimer,
			[=]()
			{
				float attackTime = 0.0f;
				const float attackGap = 0.1f;
				
				for (TWeakObjectPtr<AMonster> monster : m_TargetMonsters)
				{
					monster->GetMesh()->SetCustomDepthStencilValue(2.0f);

					FTimerHandle attackTimer;
					
					m_Owner->GetWorldTimerManager().SetTimer(
						attackTimer,
						[=]()
						{
							monster->Unpause();
							m_BattleManager->Attack(m_Owner.Get(), TEXT("FateSealed_Finish"), monster.Get());
							
							if (m_FinishSound != nullptr)
							{
								UGameplayStatics::PlaySound2D(m_Owner.Get(), m_FinishSound, 2.5f);
							}
						},
						attackTime += attackGap,
						false
						);
				}
				
				m_TargetMonsters.Empty();
			},
		1.0f,
		false);
	
}

bool UFateSealed::CanExecuteSkill() const
{
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	return Super::CanExecuteSkill() && !m_Owner->IsCrowdControlState() &&
		ownerSkillComponent->CanChargingSkill();
}
