// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AnimInstanceBase.h"
#include "Utility/Utility.h"
#include "Component/StatComponent.h"
#include "SubSystems/DataManager.h"

int32 attackCount = 0; // 로그확인용.

ACharacterBase::ACharacterBase() :
	m_CrowdControlTime(1.0f),
	m_DeathTimerTime(3.0f),
    m_WalkSpeed(200.0f),
    m_RunSpeed(400.0f),
	m_CurSpeed(0.0f),
	m_bIsSuperArmor(false),
	m_bIsDead(false)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECrowdControlType"), true);
	if (enumPtr != nullptr)
	{
		for (int i = 0; i < enumPtr->NumEnums(); ++i)
		{
			ECrowdControlType state = (ECrowdControlType)(enumPtr->GetValueByIndex(i));
			FOnCrowdControl_Delegate deletage;
			m_CrowdControlDelegates.Add(state,deletage);
		}
	}
	
	m_StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	m_StatComponent->OnHPIsZero.AddUObject(this, &ACharacterBase::ExecEvent_OnHPIsZero);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	attackCount = 0;
	
	UAnimInstanceBase* animInstance = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	animInstance->End_Death.AddUObject(this, &ACharacterBase::OnCalledNotify_End_Death);

	m_CrowdControlDelegates[ECrowdControlType::Knockback].AddUObject(this, &ACharacterBase::ExecEvent_Knockback);
	
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
		m_CrowdControlDelegates[attackInformation->crowdControlType].Broadcast(instigatorCharacter);

		if (attackInformation->knockBackDistance > 0.0f)
		{
			FVector dirToInstigator = instigatorCharacter->GetActorLocation() - this->GetActorLocation(); 
			dirToInstigator.Normalize();
			this->SetActorLocation(GetActorLocation() + dirToInstigator * -1 * attackInformation->knockBackDistance, false);
		}
	}
	
	return FinalDamage;
}

void ACharacterBase::ExecEvent_OnHPIsZero()
{
	m_bIsDead = true;
	Die();
}

void ACharacterBase::OnCalledNotify_End_Death() // 사망몽타주재생 완료시 호출.
{
	ExecEvent_EndedDeathMontage();
}

void ACharacterBase::Attack(const FName& attackName, TWeakObjectPtr<AActor> target)
{
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	const FAttackInfo* attackInfo = dataManager->GetAttackInformation(this->GetClass(), attackName);
	
	float finalDamage = m_StatComponent->GetDefaultDamage() * attackInfo->damageRatio;
	target->TakeDamage(finalDamage, *attackInfo, this->GetController(), this);
}
