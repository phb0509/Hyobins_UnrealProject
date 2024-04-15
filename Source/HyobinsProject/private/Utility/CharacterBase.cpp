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
	m_bIsAttacking(false),
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

	const FAttackInfoStruct* const attackInformation = static_cast<const FAttackInfoStruct*>(&DamageEvent);
	checkf(IsValid(DamageCauser), TEXT("DamageCauser isn't Valid"));

	m_StatComponent->SetDamage(attackInformation->damage);

	// 로그
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	if (!m_bIsDead)
	{
		m_HitDirection = Utility::GetHitDirection(this, instigatorCharacter); // 블렌드스페이스용 변수
		ExecHitEvent(instigatorCharacter); // 블랙보드에 적 입력하는용도. 이것도 메인플레이어한텐 필요없음. 오버라이딩이라 상관없구나.

		if (!m_bIsSuperArmor)
		{
			m_bIsAttacking = false; // 피격모션을 재생하기때문에 공격x. 

			// 넉백
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
	m_bIsDead = true;
	m_bIsAttacking = false;

	Die();
}

void ACharacterBase::OnCalledEndedDeathNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("OnCalled DeathNotifyEnded"));
	ExecDeathEvent();

	// 액터풀에 반환하기위한 비활성화타이머.
	GetWorldTimerManager().SetTimer(m_DeActivateTimerHandle, this, &ACharacterBase::DeActivate, m_DeathTimerTime, true);
}