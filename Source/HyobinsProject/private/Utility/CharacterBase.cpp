// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AIControllerBase.h"
#include "Utility/CustomStructs.h"
#include "Utility/Utility.h"
#include "Component/StatComponent.h"

ACharacterBase::ACharacterBase() :
	m_WalkSpeed(200.0f),
	m_RunSpeed(400.0f),
	m_HitRecovery(1.0f),
	m_OnHitTimerTime(1.0f),
	m_DeathTimerTime(3.0f),
	m_CurSpeed(0.0f),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsInAir(false),
	m_bIsSuperArmor(false),
	m_bIsDead(false),
	m_bIsHitStateTrigger(false),
	m_DeathTimerTickTime(1.0f),
	m_DeathTimerRemainingTime(3.0f),
	m_DiffuseRatio(1.0f)
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	m_StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	m_StatComponent->OnHPIsZero.AddUObject(this, &ACharacterBase::OnHPIsZero);
}

void ACharacterBase::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	const FString log = Tags[0].ToString() + " :: CharacterBase :: PossessedBy!!";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
	
	m_AIControllerBase = Cast<AAIControllerBase>(GetController());
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACharacterBase* const instigatorCharacter = Cast<ACharacterBase>(EventInstigator->GetPawn());
	checkf(IsValid(instigatorCharacter), TEXT("InstigatorCharacter isn't Valid"));

	const FAttackInfo* const attackInformation = static_cast<const FAttackInfo*>(&DamageEvent);
	checkf(IsValid(DamageCauser), TEXT("DamageCauser isn't Valid"));

	// 로그
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	m_StatComponent->SetDamage(attackInformation->damage);
	
	if (!m_bIsDead)
	{
		m_HitDirection = Utility::GetHitDirection(this, instigatorCharacter); // 피격방향을 산출.
		ExecHitEvent(instigatorCharacter); 

		if (!m_bIsSuperArmor)
		{
			// 넉백.. 코드변경해야됨. 넉백있는 공격정보일때만 수행하게
			FVector dirToInstigator = instigatorCharacter->GetActorLocation() - this->GetActorLocation(); 
			dirToInstigator.Normalize();
			this->SetActorLocation(GetActorLocation() + dirToInstigator * -1 * attackInformation->knockBackDistance, false);

			// Timer Setting.
			m_OnHitTimerTime = m_HitRecovery * attackInformation->knockBackTime;

			if (GetWorldTimerManager().IsTimerActive(m_OnHitTimerHandle))
			{
				GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
			}

			GetWorldTimerManager().SetTimer(m_OnHitTimerHandle, this, &ACharacterBase::OnHitTimerEnded, m_OnHitTimerTime, true); // OnHitTimeEnded는 각 몬스터마다 오버라이딩함수 호출.
		}
	}
	
	return FinalDamage;
}

void ACharacterBase::OnHPIsZero()
{
	m_bIsDead = true;

	Die();
}

void ACharacterBase::OnCalledNotify_EndedDeath() // 사망몽타주재생 완료시 호출.
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterBase :: OnCalled_EndedDeathNotify"));
	ExecDeathEvent();
}