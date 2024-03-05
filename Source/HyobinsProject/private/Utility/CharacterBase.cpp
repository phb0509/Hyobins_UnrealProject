// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AIControllerBase.h"
#include "Utility/AnimInstanceBase.h"
#include "Utility/Utility.h"
#include <Components/CapsuleComponent.h>
#include "HPGameInstance.h"
#include "SubSystems/UIManager.h"

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
	m_bIsAttacking(false),
	m_bIsInAir(false),
	m_bIsSuperArmor(false),
	m_bIsDeath(false),
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

	m_AIControllerBase = Cast<AAIControllerBase>(newController);
	m_AnimInstanceBase = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACharacterBase* const instigatorCharacter = Cast<ACharacterBase>(EventInstigator->GetPawn());
	checkf(IsValid(instigatorCharacter), TEXT("InstigatorCharacter isn't Valid"));

	const FAttackInfoStruct* const attackInformation = static_cast<const FAttackInfoStruct*>(&DamageEvent);
	checkf(IsValid(DamageCauser), TEXT("DamageCauser isn't Valid"));

	m_StatComponent->SetDamage(attackInformation->damage);

	// 로그
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	if (!m_bIsDeath)
	{
		m_HitDirection = Utility::GetHitDirection(this, instigatorCharacter); // 블렌드스페이스용 변수
		ExecHitEvent(instigatorCharacter);

		if (!m_bIsSuperArmor)
		{
			SetCommonState(EMonsterCommonStates::Hit); // 몽타주 재생 및, curState이랑 블랙보드에 Hit상태 기록.

			m_bIsAttacking = false; // 피격모션을 재생하기때문에 공격x. 

			// 거리넉백을 위한 속도 계산.
			FVector dirToInstigator = instigatorCharacter->GetActorLocation() - this->GetActorLocation(); 
			dirToInstigator.Normalize();
			this->SetActorLocation(GetActorLocation() + dirToInstigator * -1 * attackInformation->knockBackDistance, false);

			// Timer Setting.
			m_OnHitTimerTime = m_HitRecovery * attackInformation->knockBackTime;

			if (GetWorldTimerManager().IsTimerActive(m_OnHitTimerHandle))
			{
				GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
			}

			GetWorldTimerManager().SetTimer(m_OnHitTimerHandle, this, &ACharacterBase::OnHitTimerEnded, m_OnHitTimerTime, true); // OnHitTimeEnded는 알아서 오버라이드되서 호출.
		}
	}
	
	return FinalDamage;
}

void ACharacterBase::OnHPIsZero()
{
	m_bIsDeath = true;
	m_bIsAttacking = false;

	Die();
}

void ACharacterBase::OnCalledDeathMontageEndedNotify()
{
	ExecDeathEvent();

	// 액터풀에 반환하기위한 비활성화타이머.
	GetWorldTimerManager().SetTimer(m_DeActivateTimerHandle, this, &ACharacterBase::DeActivate, m_DeathTimerTime, true);
}