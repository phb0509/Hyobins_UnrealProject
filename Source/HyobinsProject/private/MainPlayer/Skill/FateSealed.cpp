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
	m_FinishAttackDelayGap(0.1f),
	m_AttackDelay(0.1f),
	m_FinishAttackCallTime(0.0f)
{
	
}

void UFateSealed::Initialize()
{
	Super::Initialize();

	check(m_FateSealedMontage != nullptr);
	check(m_FateSealedFinishMontage != nullptr);
	
	m_BattleManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UBattleManager>();
	check(m_BattleManager != nullptr);
	
	m_DynamicPPMaterial = UMaterialInstanceDynamic::Create(m_PPMaterial, this);
	check(m_DynamicPPMaterial != nullptr);

	check(m_FinishSound != nullptr);
	
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
	
}

void UFateSealed::Execute()
{
	Super::Execute();

	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::FateSealed);
	
	APostProcessVolume* postProcessVolume = m_Owner->GetGlobalPostProcessVolume();
	check(postProcessVolume != nullptr);

	postProcessVolume->bUnbound = true;
	postProcessVolume->BlendWeight = 1.0f;
	postProcessVolume->Settings.AddBlendable(m_PPMaterial.Get(), 1);
	
	m_FinishAttackCallTime = 0.0f;
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

		if (targetMonster != nullptr)
		{
			targetMonster->GetMesh()->SetCustomDepthStencilValue(2);
			targetMonster->Pause();
			
			m_TargetMonsters.Add(targetMonster);
		}
	}
	
	m_CurTargetIndex = 0;
	moveToNextTarget();
}


void UFateSealed::moveToNextTarget()
{
	if (m_CurTargetIndex >= m_TargetMonsters.Num())
	{
		finishSkill();
		
		return;
	}
	
	m_CurTarget = m_TargetMonsters[m_CurTargetIndex].Get();
	
	if (m_CurTarget.IsValid())
	{
		const FVector targetLocation = m_CurTarget->GetActorLocation();
		const FVector finalLocation = targetLocation + (m_Owner->GetActorLocation() - targetLocation).GetSafeNormal() * 30.0f;

		m_Owner->SetActorLocation(finalLocation);
		m_Owner->RotateToTarget(m_CurTarget.Get());

		const FName section = *FString::FromInt(m_CurTargetIndex % 5);
		m_OwnerAnimInstance->Montage_Play(m_FateSealedMontage,1.0f);
		m_OwnerAnimInstance->Montage_JumpToSection(section, m_FateSealedMontage);
	}
}

void UFateSealed::onFateSealedEnded() // 공격몽타주 재생 이후,
{
	if (m_CurTarget.IsValid())
	{
		m_StencilTargets.Add(m_CurTarget);
		
		if (m_StencilTargets.Num() == 1)
		{
			m_Owner->GetWorldTimerManager().SetTimer(
				m_StencilUpdateTimer,
				this,
				&UFateSealed::updateAllStencilValues,
				0.01f,
				true,
				0.0f
			);
		}
	}
	
	++m_CurTargetIndex;

	moveToNextTarget();
}


void UFateSealed::updateAllStencilValues()
{
	for (int32 i = m_StencilTargets.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<AMonster> monster = m_StencilTargets[i];
        
		if (!monster.IsValid())
		{
			m_StencilTargets.RemoveAtSwap(i);
			
			continue;
		}
		
		int32 newStencil = monster->GetMesh()->CustomDepthStencilValue + 1;
        
		monster->GetMesh()->SetCustomDepthStencilValue(newStencil);
		
		if (newStencil >= 102)
		{
			m_StencilTargets.RemoveAtSwap(i);
		} 
	}
	
	if (m_StencilTargets.Num() == 0)
	{
		m_Owner->GetWorldTimerManager().ClearTimer(m_StencilUpdateTimer);
		
		m_StencilTargets.Empty();
	}
}


void UFateSealed::finishSkill()
{
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	check(ownerSkillComponent != nullptr);
	
	ownerSkillComponent->SetCanChargingSkill(false);
	
	m_Owner->SetActorLocation(m_SkillStartLocation);
	m_OwnerAnimInstance->Montage_Play(m_FateSealedFinishMontage,1.0f);
}

void UFateSealed::onFateSealedFinishEnded()
{
	APostProcessVolume* postProcessVolume = m_Owner->GetGlobalPostProcessVolume();
	check(postProcessVolume != nullptr);
	
	postProcessVolume->bUnbound = false;
	postProcessVolume->Settings.RemoveBlendable(m_PPMaterial.Get());
	
	for (TWeakObjectPtr<AMonster> monster : m_TargetMonsters)
	{
		if (monster.IsValid())
		{
			monster->GetMesh()->SetCustomDepthStencilValue(0);
		}
	}
	
	FTimerHandle finishTimer;
	m_Owner->GetWorldTimerManager().SetTimer
		(
			finishTimer,
			[this]() 
			{
				executeStaggeredAttack();
				
				m_TargetMonsters.Empty();
			},
		m_FinishAttackDelay,
		false
		);

	m_Owner->RemoveInputMappingContext(TEXT("Charging_OnGround"));
	m_Owner->AddInputMappingContext(TEXT("Default_OnGround")); 
	m_Owner->SetIsSuperArmor(false);
}

void UFateSealed::executeStaggeredAttack()
{
	for (TWeakObjectPtr<AMonster> monster : m_TargetMonsters)
	{
		if (!monster.IsValid())
		{
			continue;	
		}
					
		// 실제 공격적용타이머 특정시간 간격으로 셋팅
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
			m_FinishAttackCallTime += m_FinishAttackDelayGap,
			false
						
		);
	}
}

bool UFateSealed::CanExecuteSkill() const
{
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	
	return Super::CanExecuteSkill() && !m_Owner->IsCrowdControlState() &&
		ownerSkillComponent->CanChargingSkill();
}
