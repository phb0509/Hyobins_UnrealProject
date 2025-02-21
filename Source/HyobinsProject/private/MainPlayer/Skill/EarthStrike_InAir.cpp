// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/EarthStrike_InAir.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
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

	if (!m_bIsCoolDownActive)
	{
		return;
	}
	
	m_bIsCoolDownActive = false;
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetState();
	
	OnExecute.Broadcast();
	
	m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::EarthStrike_FallingToGround);
	m_OwnerAnimInstance->PlayMontage(TEXT("EarthStrike_FallingToGround"));
	m_Owner->GetCharacterMovement()->GravityScale = 6.0f;
		
	m_Owner->GetWorldTimerManager().SetTimer
	(
	  m_Timer,
	this,
		&UEarthStrike_InAir::ExecEventIfOnGround,
		m_Owner->GetWorld()->DeltaTimeSeconds,
		true, -1
	);
}

void UEarthStrike_InAir::ExecEventIfOnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		m_Owner->GetWorldTimerManager().ClearTimer(m_Timer);
		m_OwnerAnimInstance->PlayMontage(TEXT("EarthStrike_OnGround"));

		Attack();
		PlayEffect();
		UGameplayStatics::PlaySound2D(m_Owner.Get(), m_Sound, 1.0f);
	}
}

void UEarthStrike_InAir::Attack()
{
	const FVector startLocation = m_Owner->GetCollider(TEXT("ShieldBottomCollider"))->GetComponentLocation();
	
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)};
	const TArray<AActor*> IgnoreActors = {m_Owner.Get()};
	TArray<AActor*> overlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(m_Owner->GetWorld(),
		startLocation,
		500.0f, // 구체 반지름
		ObjectTypes,
		nullptr,
		IgnoreActors,
		overlappedActors);
	
	if (overlappedActors.Num() > 0)
	{
		for (AActor* overlappedEnemy : overlappedActors)
		{
			const ACharacterBase* enemy = Cast<ACharacterBase>(overlappedEnemy);
			if (enemy->GetIsOnGround())
			{
				m_Owner->Attack(TEXT("EarthStrike"), overlappedEnemy);
			}
		}
	}	
}

void UEarthStrike_InAir::PlayEffect()
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
