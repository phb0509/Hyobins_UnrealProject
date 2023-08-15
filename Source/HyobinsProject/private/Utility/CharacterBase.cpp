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

	m_StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat"));
	m_StatComponent->OnHPIsZero.AddUObject(this, &ACharacterBase::OnHPIsZero);

}

void ACharacterBase::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	FString temp = Tags[0].ToString() + " :: CharacterBase :: Possessedby!!";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *temp);

	m_AIControllerBase = Cast<AAIControllerBase>(newController);
	m_AnimInstanceBase = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACharacterBase* instigatorCharacter = Cast<ACharacterBase>(EventInstigator->GetPawn());
	checkf(IsValid(instigatorCharacter), TEXT("InstigatorCharacter is not Valid"));

	const FAttackInfoStruct* attackInformation = static_cast<const FAttackInfoStruct*>(&DamageEvent);
	checkf(IsValid(DamageCauser), TEXT("DamageCauser is not Valid"));

	m_StatComponent->SetDamage(attackInformation->damage);

	// �α�.
	FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	if (!m_bIsDeath)
	{
		m_HitDirection = Utility::GetHitDirection(this, instigatorCharacter);
		ExecHitEvent(instigatorCharacter);

		if (!m_bIsSuperArmor)
		{
			SetCommonState(EMonsterCommonStates::Hit); // ��Ÿ�� ��� ��, curState�̶� �����忡 Hit���� ���.

			m_bIsAttacking = false; // ���۾ƸӰ��� ���°� �ƴϸ� �ǰݽ� ���� �������°� �Ǵϱ� attacking�� false�� ����� �Ѵ�.

			FVector dirToInstigator = instigatorCharacter->GetActorLocation() - this->GetActorLocation();
			dirToInstigator.Normalize();
			this->SetActorLocation(GetActorLocation() + dirToInstigator * -1 * attackInformation->knockBackDistance, false);

			// Timer Setting.
			m_OnHitTimerTime = m_HitRecovery * attackInformation->knockBackTime;

			if (GetWorldTimerManager().IsTimerActive(m_OnHitTimerHandle))
			{
				GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
			}

			GetWorldTimerManager().SetTimer(m_OnHitTimerHandle, this, &ACharacterBase::OnHitTimerEnded, m_OnHitTimerTime, true); // OnHitTimeEnded�� �˾Ƽ� �������̵�Ǽ� ȣ���.
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

	// ����Ǯ�� ��ȯ�ϱ����� ��Ȱ��ȭŸ�̸�
	GetWorldTimerManager().SetTimer(m_DeActivateTimerHandle, this, &ACharacterBase::DeActivate, m_DeathTimerTime, true);
}

void ACharacterBase::initAttackInformations(FString path)
{
	auto HPGameInstance = Cast<UHPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	HPGameInstance->InitAttackInformations(path, m_AttackInformations);
}