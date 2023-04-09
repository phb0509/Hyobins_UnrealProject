// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"

ACharacterBase::ACharacterBase() :
	m_MaxHP(100.0f),
	m_CurHP(m_MaxHP),
	m_CurSpeed(0.0f),
	m_WalkSpeed(200.0f),
	m_RunSpeed(400.0f),
	m_HitRecovery(1.0f),
	m_OnHitTimerTime(1.0f),
	m_DeathTimerTime(3.0f),
	m_bIsActivated(false),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsAttacking(false),
	m_bIsInAir(false),
	m_bIsSuperArmor(false),
	m_bIsDeath(false)
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // �������Ʈ�� ����ϰڴٴ� �ǹ�.
}

void ACharacterBase::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
}

void ACharacterBase::initAttackInformations(FString path)
{
	auto HPGameInstance = Cast<UHPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	HPGameInstance->InitAttackInformations(path, m_AttackInformations);
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (m_bIsDeath)
	{
		return 0.0f;
	}

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACharacterBase* instigatorCharacter = Cast<ACharacterBase>(EventInstigator->GetPawn());
	const FAttackInfoStruct* attackInformation = static_cast<const FAttackInfoStruct*>(&DamageEvent);

	checkf(IsValid(instigatorCharacter), TEXT("InstigatorCharacter is not Valid"));
	checkf(IsValid(DamageCauser), TEXT("DamageCauser is not Valid"));

	m_CurHP -= attackInformation->damage;

	// �α�.
	FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	if (m_CurHP <= 0)
	{
		m_bIsDeath = true;
		m_bIsAttacking = false;

		Die();
	}
	else
	{
		ExecHitEvent(instigatorCharacter);
		
		if (!m_bIsSuperArmor)
		{
			SetHitState(); // ��Ÿ�� ��� ��, curState�̶� �����忡 Hit���� ���.

			m_bIsAttacking = false; // ���۾ƸӰ��� ���°� �ƴϸ� �ǰݽ� ���� �������°� �Ǵϱ� attacking�� false�� ����� �Ѵ�.

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

void ACharacterBase::OnCalledDeathMontageEndedNotify()
{
	GetWorldTimerManager().SetTimer(m_DeathTimerHandle, this, &ACharacterBase::DeathTimerEnded, m_DeathTimerTime, true); // OnHitTimeEnded�� �˾Ƽ� �������̵�Ǽ� ȣ���.
}

void ACharacterBase::DeathTimerEnded()
{
	GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
	DeActivate();
}

void ACharacterBase::Activate()
{
	m_bIsActivated = true;
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	GetController()->Possess(this);
}

void ACharacterBase::DeActivate()
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterBase :: DeActivate"));

	m_bIsActivated = false;
	SetActorTickEnabled(false); 
	SetActorHiddenInGame(true);
}