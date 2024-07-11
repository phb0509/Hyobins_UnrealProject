// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AnimInstanceBase.h"
#include "Utility/CustomStructs.h"
#include "Utility/Utility.h"
#include "Component/StatComponent.h"

int32 attackCount = 0;

ACharacterBase::ACharacterBase() :
	m_WalkSpeed(200.0f),
	m_RunSpeed(400.0f),
	m_HitRecovery(1.0f),
	m_OnHitTimerTime(1.0f),
	m_DeathTimerTime(3.0f),
	m_CurSpeed(0.0f),
	m_bIsSuperArmor(false),
	m_bIsDead(false),
	m_DeathTimerTickTime(1.0f),
	m_DeathTimerRemainingTime(3.0f),
	m_DiffuseRatio(1.0f)
{
	m_StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	m_StatComponent->OnHPIsZero.AddUObject(this, &ACharacterBase::OnHPIsZero);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	animInstance->End_Death.AddUObject(this, &ACharacterBase::OnCalledNotify_End_Death);

	attackCount = 0;
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACharacterBase* const instigatorCharacter = Cast<ACharacterBase>(EventInstigator->GetPawn());
	checkf(IsValid(instigatorCharacter), TEXT("InstigatorCharacter isn't Valid"));

	const FAttackInfo* const attackInformation = static_cast<const FAttackInfo*>(&DamageEvent);
	checkf(IsValid(DamageCauser), TEXT("DamageCauser isn't Valid"));

	++attackCount;
	// 로그
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(DamageAmount) + " damage from " +
		instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString() + "::" + FString::FromInt(attackCount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	m_StatComponent->SetDamage(DamageAmount);
	
	if (!m_bIsDead)
	{
		m_HitDirection = Utility::GetHitDirection(this, instigatorCharacter); // 피격방향을 산출.
		ExecOnHitEvent(instigatorCharacter); 

		if (!m_bIsSuperArmor)
		{
			if (attackInformation->bHasKnockBack) // 넉백을 주는 공격이라면
			{
				FVector dirToInstigator = instigatorCharacter->GetActorLocation() - this->GetActorLocation(); 
				dirToInstigator.Normalize();
				this->SetActorLocation(GetActorLocation() + dirToInstigator * -1 * attackInformation->knockBackDistance, false);
			}
			
			// Timer Setting.
			m_OnHitTimerTime = m_HitRecovery * attackInformation->knockBackTime;
			GetWorldTimerManager().SetTimer(m_OnHitTimerHandle, this, &ACharacterBase::OnCalledTimer_EndedOnHitKnockback, m_OnHitTimerTime, false); 
		}
	}
	
	return FinalDamage;
}

void ACharacterBase::OnHPIsZero()
{
	m_bIsDead = true;

	Die();
}

void ACharacterBase::OnCalledNotify_End_Death() // 사망몽타주재생 완료시 호출.
{
	ExecEvent_EndedDeathMontage();
}