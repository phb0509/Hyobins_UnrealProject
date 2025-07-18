// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase/CharacterBase.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Utility/Utility.h"
#include "Utility/CustomStructs.h"
#include "Utility/EnumTypes.h"
#include "Component/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "SubSystems/DataManager.h"
#include "MotionWarpingComponent.h"
#include "Component/CrowdControlComponent.h"


int32 attackCount = 0; // 로그용 임시값
const FName ACharacterBase::HitColliderName = "HitCollider";
const FName ACharacterBase::KnockbackMontageNames[4] = {"Knockback0", "Knockback1", "Knockback2", "Knockback3"};
const FName ACharacterBase::DeathMontageNames[4] = {"Death0", "Death1", "Death2", "Death3"};


ACharacterBase::ACharacterBase() :
    m_WalkSpeed(200.0f),
    m_RunSpeed(400.0f),
	m_CurSpeed(0.0f),
	m_bIsSuperArmor(false),
	m_bIsDead(false)
{
	m_StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	m_StatComponent->OnHPIsZero.AddUObject(this, &ACharacterBase::OnHPIsZero);
	m_StatComponent->OnStaminaIsZero.AddUObject(this, &ACharacterBase::OnStaminaIsZero);

	m_CrowdControlComponent = CreateDefaultSubobject<UCrowdControlComponent>(TEXT("CrowdControlComponent"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	attackCount = 0;
	
	m_AnimInstanceBase = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	m_AnimInstanceBase->End_Death.AddUObject(this, &ACharacterBase::OnCalledNotify_End_Death); 
	
	this->OnTakeDamage.AddUObject(this, &ACharacterBase::PlayOnHitEffect);
}

void ACharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_CurSpeed = GetVelocity().Size();
	m_bIsOnGround = GetCharacterMovement()->IsMovingOnGround();
	m_bIsFalling = GetCharacterMovement()->IsFalling();
	m_bIsFlying = GetCharacterMovement()->IsFlying();
	
}

void ACharacterBase::Attack(const FName& attackName, AActor* target, const FVector& causerLocation)
{
	if (!IsValid(target))
	{
		return;
	}
	
	IDamageable* damageableActor = Cast<IDamageable>(target);

	if (damageableActor == nullptr)
	{
		return;
	}
	
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	const FAttackInformation* attackInfo = dataManager->GetAttackInformation(this->GetClass(), attackName);
	
	const bool bIsCriticalAttack = FMath::FRandRange(0.0f, 100.0f) <= m_StatComponent->GetCriticalAttackChance();
	const float damage = (m_StatComponent->GetDefaultDamage() * attackInfo->damageRatio) * (bIsCriticalAttack ? 2.0f : 1.0f);
	
	damageableActor->OnDamage(damage, bIsCriticalAttack, attackInfo, this, causerLocation);
}

void ACharacterBase::OnDamage(const float damage, const bool bIsCriticalAttack,
	const FAttackInformation* attackInfo, AActor* instigator, const FVector& causerLocation)
{
	const ECrowdControlType crowdControlType = attackInfo->crowdControlType;
	const float finalCrowdControlTime = m_StatComponent->CalculateFinalCrowdControlTime(attackInfo->crowdControlTime);
	const float finalDamage = m_StatComponent->CalculateFinalDamage(damage); 

	m_StatComponent->OnDamageHP(finalDamage);
	m_LastHitDirection = Utility::GetHitDirection(instigator->GetActorLocation(), this); // 피격방향 계산.
	
	// 피격정보 생성.
	const FHitInformation hitInfo =
	{
		attackInfo->attackName,
		instigator,
		this,
		finalDamage,
		bIsCriticalAttack,
		m_LastHitDirection,
		crowdControlType,
		finalCrowdControlTime,
		attackInfo->knockBackDistance,
		attackInfo->airbornePower
	};
	
	OnTakeDamage.Broadcast(hitInfo); // 데미지UI 및 HitEffect
	execEvent_CommonCrowdControl(instigator);
	
	if (!m_bIsDead && !m_bIsSuperArmor)
	{
		if (crowdControlType != ECrowdControlType::None)
		{
			m_CrowdControlComponent->ApplyCrowdControl(instigator, hitInfo);
		}
	}
	
	// 로그용
	++attackCount;
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(finalDamage) + " damage from " +
		instigator->Tags[0].ToString() + "::" + attackInfo->attackName.ToString() + "::" + FString::FromInt(attackCount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
}

void ACharacterBase::OnDamageStamina(const float staminaDamage) const
{
	m_StatComponent->OnDamageStamina(staminaDamage);
	
}

void ACharacterBase::OnHPIsZero()
{
	Die();
}

void ACharacterBase::Die()
{
	m_bIsDead = true;
	m_CrowdControlComponent->ClearCrowdControlTimerHandle();
	m_StatComponent->SetCanRecoveryHP(false);
	m_StatComponent->SetCanRecoveryStamina(false);
	
	OnDeath.Broadcast();
	
	playDeathMontage(0);

	
	UShapeComponent* hitCollider = m_Colliders[HitColliderName].Get();
	if (hitCollider != nullptr)
	{
		hitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACharacterBase::OnCalledNotify_End_Death() 
{
	ExecEvent_EndedDeathMontage();
}

void ACharacterBase::playDeathMontage(const int32 hitDirection)
{
	m_AnimInstanceBase->StopAllMontages(0.0f);
	
	if (m_DeathMontages.Num() >= 1) // 최소 1개이상있으면
	{
		if (m_DeathMontages.Num() > hitDirection)
		{
			m_AnimInstanceBase->Montage_Play(m_DeathMontages[hitDirection]);
		}
		else
		{
			m_AnimInstanceBase->Montage_Play(m_DeathMontages[0]);
		}
	}
}

void ACharacterBase::PlayOnHitEffect(const FHitInformation& hitInformation)
{
	if (m_HitSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, m_HitSound, GetActorLocation());
	}
}

void ACharacterBase::ApplyKnockback(const float knockbackDistance, const FVector& instigatorLocation)
{
	// FVector dirToInstigator = instigatorLocation - this->GetActorLocation(); 
	// dirToInstigator.Normalize();
	// dirToInstigator *= -1 * knockbackDistance;
	// dirToInstigator.Z = 0.0f;
	//
	// this->SetActorLocation(GetActorLocation() + dirToInstigator, false);
}

void ACharacterBase::RotateToTarget(const AActor* target, const FRotator& rotatorOffset)
{
	const FVector targetLocation = target->GetActorLocation();
    const FVector myLocation = this->GetActorLocation();
	
    const FVector directionToTarget = (targetLocation - myLocation).GetSafeNormal();
    const FRotator rotationToTarget = directionToTarget.Rotation();
	
	FRotator curRotation = this->GetActorRotation();
	curRotation.Yaw = rotationToTarget.Yaw;
	curRotation += rotatorOffset;
	
    SetActorRotation(curRotation);
}

bool ACharacterBase::IsWithInRange(const AActor* target, const float range) const
{
	if (!IsValid(target))
	{
		return false;
	}
	
	const FVector myLocation = this->GetActorLocation();
	const FVector targetLocation = target->GetActorLocation();
	
	float distance = FVector::DistSquared(myLocation, targetLocation);
	
	return distance <= range * range;
}

FVector ACharacterBase::GetDirectionToTarget(const AActor* target) const
{
	FVector directionToTarget = target->GetActorLocation() - this->GetActorLocation();
	directionToTarget.Z = 0.0f;

	return directionToTarget;
}

bool ACharacterBase::HasEnoughStamina(const float cost) const
{
	return m_StatComponent->HasEnoughStamina(cost);
}

UAnimInstanceBase* ACharacterBase::GetAnimInstanceBase() const
{
	return m_AnimInstanceBase.Get();
}

UMotionWarpingComponent* ACharacterBase::GetMotionWarpingComponent() const
{
	return m_MotionWarpingComponent.Get();
}

void ACharacterBase::BreakCrowdControlState()
{
	m_CrowdControlComponent->BreakCrowdControlState();
}

void ACharacterBase::SetInvincible(const bool bIsInvincible)
{
	if (m_HitCollider != nullptr)
	{
		if (bIsInvincible)
		{
			m_HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

UShapeComponent* ACharacterBase::GetCollider(const FName& colliderName) const
{
	return m_Colliders.Contains(colliderName) ? m_Colliders[colliderName].Get() : nullptr;
}

bool ACharacterBase::IsCrowdControlState() const
{
	return m_CrowdControlComponent->IsCrowdControlState();
}
