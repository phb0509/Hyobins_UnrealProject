// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/EarthStrike_InAir.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/SkillComponent.h"
#include "Utility/EnumTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystems/BattleManager.h"

UEarthStrike_InAir::UEarthStrike_InAir() :
	m_FallingToGroundMontage(nullptr),
	m_EarthStrikeMontage(nullptr),
	m_AttackRangeRadius(500.0f),
	m_Particle(nullptr),
	m_CameraShake(nullptr),
	m_Sound(nullptr)
{
}

void UEarthStrike_InAir::Initialize()
{
	Super::Initialize();

	check(m_FallingToGroundMontage != nullptr);
	check(m_EarthStrikeMontage != nullptr);
	check(m_Particle != nullptr);
	check(m_CameraShake != nullptr);
	check(m_Sound != nullptr);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("EarthStrike_OnGround"),
[this]()
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	});
}

void UEarthStrike_InAir::Execute()
{
	Super::Execute();
	
	m_Owner->GetCharacterMovement()->GravityScale = 6.0f;
	m_Owner->GetWorldTimerManager().SetTimer
	(
		m_Timer,
		this,
		&UEarthStrike_InAir::ExecEvent_WhenOnGround,
		m_Owner->GetWorld()->DeltaTimeSeconds,
		true, -1
	);
	
	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::EarthStrike_FallingToGround);
	
	m_OwnerAnimInstance->Montage_Play(m_FallingToGroundMontage, 1.0f);
}

void UEarthStrike_InAir::ExecEvent_WhenOnGround()
{
	if (m_Owner->IsOnGround())
	{
		m_Owner->GetWorldTimerManager().ClearTimer(m_Timer);
		m_OwnerAnimInstance->Montage_Play(m_EarthStrikeMontage, 1.0f);

		attack();
		playEffect();
	}
}

void UEarthStrike_InAir::attack()
{
	UBattleManager* battleManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UBattleManager>();
	check(battleManager != nullptr);
	
	const FVector startLocation = m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation();
	
	TArray<FHitResult> hitResults;
	FCollisionShape sphereShape = FCollisionShape::MakeSphere(m_AttackRangeRadius);
	FCollisionQueryParams params;
	FCollisionObjectQueryParams objectParams = battleManager->MakeCollisionObjectParams(m_Owner.Get());
	
	bool bHit = m_Owner->GetWorld()->SweepMultiByObjectType(
		hitResults,
		startLocation,
		startLocation,
		FQuat::Identity,
		objectParams, 
		sphereShape,
		params
	);
		
	if (bHit)
	{
		for (const FHitResult& hitResult : hitResults)
		{
			ACharacterBase* hitActor = Cast<ACharacterBase>(hitResult.GetActor());
			const FName atatckName = TEXT("EarthStrike");
				
			if (hitActor != nullptr && !m_Owner->HasContainHitActor(atatckName, hitActor))
			{
				//m_Owner->AddHitActorsByMe(atatckName, hitActor);
				battleManager->Attack(m_Owner.Get(), atatckName, hitActor, hitResult.Location);
			}
		}
	}
}

void UEarthStrike_InAir::playEffect()
{
	if (m_Particle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
		   m_Owner->GetWorld(),
		   m_Particle, 
		   m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation(), 
		   FRotator(0.0f)  
	   );
	}
	
	if (m_CameraShake != nullptr)
	{
		m_Owner->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_CameraShake);
	}

	if (m_Sound != nullptr)
	{
		UGameplayStatics::PlaySound2D(m_Owner.Get(), m_Sound, 1.0f);
	}
}

bool UEarthStrike_InAir::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() &&
		!m_Owner->IsCrowdControlState();
}
