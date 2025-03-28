// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/EarthStrike_InAir.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Interfaces/Damageable.h"
#include "Utility/EnumTypes.h"
#include "Kismet/GameplayStatics.h"

UEarthStrike_InAir::UEarthStrike_InAir() :
	m_AttackRangeRadius(500.0f),
	m_Particle(nullptr),
	m_CameraShake(nullptr),
	m_Sound(nullptr)
{
}

void UEarthStrike_InAir::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("EarthStrike_OnGround"),
[this]()
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	});
}

void UEarthStrike_InAir::Execute()
{
	Super::Execute();
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetSkillState();
	
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
	
	m_OwnerAnimInstance->PlayMontage(TEXT("EarthStrike_FallingToGround"));
}

void UEarthStrike_InAir::ExecEvent_WhenOnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		m_Owner->GetWorldTimerManager().ClearTimer(m_Timer);
		m_OwnerAnimInstance->PlayMontage(TEXT("EarthStrike_OnGround"));

		attack();
		playEffect();
		UGameplayStatics::PlaySound2D(m_Owner.Get(), m_Sound, 1.0f);
	}
}

void UEarthStrike_InAir::attack()
{
	const FVector startLocation = m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation();
	
	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)};
	const TArray<AActor*> ignoreActors = {m_Owner.Get()};
	TArray<AActor*> overlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(m_Owner->GetWorld(),
		startLocation,
		500.0f, // 구체 반지름
		objectTypes,
		nullptr,
		ignoreActors,
		overlappedActors);
	
	if (overlappedActors.Num() > 0)
	{
		for (AActor* overlappedEnemy : overlappedActors)
		{
			if (overlappedEnemy != nullptr)
			{
				m_Owner->Attack(TEXT("EarthStrike"), overlappedEnemy);
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
}

bool UEarthStrike_InAir::GetCanExecuteSkill() const
{
	return !m_Owner->GetIsCrowdControlState();
}
