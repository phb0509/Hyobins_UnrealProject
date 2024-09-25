// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AnimInstanceBase.h"
#include "Utility/Utility.h"
#include "Utility/CustomStructs.h"
#include "Utility/EnumTypes.h"
#include "Component/StatComponent.h"
#include "SubSystems/DataManager.h"


int32 attackCount = 0; // 로그확인용.

ACharacterBase::ACharacterBase() :
	m_CrowdControlTime(1.0f),
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
			FOnCrowdControl_Start_Delegate startDelegate;
			m_CrowdControl_Start_Delegates.Add(state,startDelegate);

			// FOnCrowdControl_End_Delegate endDelegate;
			// m_CrowdControl_End_Delegates.Add(state,endDelegate);
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
	animInstance->End_Death.AddUObject(this, &ACharacterBase::OnCalledNotify_End_Death); // Death는 Ended를 호출하지않기에, 노티파이 심어야함.
	animInstance->End_GetUp.AddUObject(this, &ACharacterBase::OnCalledNotify_End_GetUp); 
	
	m_CrowdControl_Start_Delegates[ECrowdControlType::Knockback].AddUObject(this, &ACharacterBase::ExecEvent_TakeKnockbackAttack);
	m_CrowdControl_Start_Delegates[ECrowdControlType::Airborne].AddUObject(this, &ACharacterBase::ExecEvent_TakeAirborneAttack);
	m_CrowdControl_Start_Delegates[ECrowdControlType::Groggy].AddUObject(this, &ACharacterBase::ExecEvent_TakeGroggyAttack);
	
	//m_CrowdControl_End_Delegates[ECrowdControlType::Knockback].AddUObject(this, &ACharacterBase::OnCalledTimer_KnockbackOnStanding_End);
	//m_CrowdControl_End_Delegates[ECrowdControlType::Groggy].AddUObject(this, &ACharacterBase::OnCalledTimer_Groggy_End);
}

void ACharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_CurSpeed = GetVelocity().Size();
	m_bIsOnGround = GetCharacterMovement()->IsMovingOnGround();
	m_bIsFalling = GetCharacterMovement()->IsFalling();
	m_bIsFlying = GetCharacterMovement()->IsFlying();
}

void ACharacterBase::Attack(const FName& attackName, TWeakObjectPtr<AActor> target)
{
	ACharacterBase* targetActor = Cast<ACharacterBase>(target);
	const UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	const FAttackInformation* attackInfo = dataManager->GetAttackInformation(this->GetClass(), attackName);
	
	const bool bIsCriticalAttack = FMath::FRandRange(0, 100) <= m_StatComponent->GetCriticalAttackChance();
	const float finalDamage = (m_StatComponent->GetDefaultDamage() * attackInfo->damageRatio) * (bIsCriticalAttack ? 2.0f : 1.0f);
	
	targetActor->OnDamage(finalDamage, bIsCriticalAttack, attackInfo, this);
}

void ACharacterBase::OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation* attackInfo, const ACharacterBase* instigator)
{
	const FHitInformation hitInfo = { attackInfo->attackName, this->Tags[0], this->GetActorLocation(), damage, bIsCriticalAttack};
	
	OnTakeDamage.Broadcast(hitInfo);
	m_StatComponent->SetDamage(damage);
	
	if (!m_bIsDead)
	{
		if (attackInfo->bHasCrowdControl)
		{
			execEvent_CommonCrowdControl(instigator);
			
			ECrowdControlType crowdControl = attackInfo->crowdControlType;
			m_HitDirection = Utility::GetHitDirection(this, instigator); // 피격방향을 산출.
			m_CrowdControl_Start_Delegates[crowdControl].Broadcast(instigator, attackInfo);
		
			if (attackInfo->knockBackDistance > 0.0f)
			{
				FVector dirToInstigator = instigator->GetActorLocation() - this->GetActorLocation(); 
				dirToInstigator.Normalize();
				dirToInstigator *= -1 * attackInfo->knockBackDistance;
				dirToInstigator.Z = 0.0f;
				this->SetActorLocation(GetActorLocation() + dirToInstigator, false);
			}
		
			m_CrowdControlTime = m_StatComponent->GetHitRecovery() * attackInfo->crowdControlTime;
		}
	}

	// 로그
	++attackCount;
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(damage) + " damage from " +
		instigator->Tags[0].ToString() + "::" + attackInfo->attackName.ToString() + "::" + FString::FromInt(attackCount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
}

void ACharacterBase::ExecEvent_OnHPIsZero()
{
	m_bIsDead = true;
	Die();
}

void ACharacterBase::OnCalledNotify_End_Death() 
{
	ExecEvent_EndedDeathMontage();
}


