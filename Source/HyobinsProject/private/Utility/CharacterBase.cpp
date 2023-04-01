// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"

ACharacterBase::ACharacterBase() :
	m_MaxHP(100.0f),
	m_CurHP(m_MaxHP),
	m_CurSpeed(0.0f),
	m_WalkSpeed(200.0f),
	m_RunSpeed(400.0f),
	m_HitRecovery(1.0f),
	m_OnHitTimer(1.0f),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsAttacking(false),
	m_bIsInAir(false),
	m_bIsSuperArmor(false),
	m_bIsDeath(false)
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); // 블루프린트를 사용하겠다는 의미.
}

void ACharacterBase::LoadMesh(FString assetPath)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		mesh(*assetPath);

	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
}

void ACharacterBase::LoadAnimInstance(FString assetPath)
{
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		animInstance(*assetPath);

	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
}

void ACharacterBase::initAttackInformations(FString path)
{
	//FString dataPath = "DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'";

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
	checkf(IsValid(instigatorCharacter), TEXT("instigatorCharacter is not Valid"));
	checkf(IsValid(DamageCauser), TEXT("DamageCauser is not Valid"));
	const FAttackInfoStruct* attackInformation = static_cast<const FAttackInfoStruct*>(&DamageEvent);

	m_CurHP -= attackInformation->damage;

	if (m_CurHP <= 0)
	{
		Die();
	}

	if (!m_bIsSuperArmor)
	{
		SetHitState();

		m_bIsAttacking = false; // 슈퍼아머같은 상태가 아니면 피격시 강제 온힛상태가 되니까 attacking을 false로 해줘야 한다.

		// Timer Setting.
		m_OnHitTimer = m_HitRecovery * attackInformation->knockBackTime;

		if (GetWorldTimerManager().IsTimerActive(m_OnHitTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
		}

		GetWorldTimerManager().SetTimer(m_OnHitTimerHandle, this, &ACharacterBase::OnHitTimerEnded, m_OnHitTimer, true); // OnHitTimeEnded는 알아서 오버라이드되서 호출됨.
	}

	// 로그.
	FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	return FinalDamage;
}

void ACharacterBase::OnHitTimerEnded()
{
	this->OnHitTimerEnded();
}

void ACharacterBase::Die()
{
	this->Die();
}