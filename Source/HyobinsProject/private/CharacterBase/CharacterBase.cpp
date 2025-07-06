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


int32 attackCount = 0; // 로그용 임시값
const FName ACharacterBase::HitColliderName = "HitCollider";
const FName ACharacterBase::KnockbackMontageNames[4] = {"Knockback0", "Knockback1", "Knockback2", "Knockback3"};
const FName ACharacterBase::DeathMontageNames[4] = {"Death0", "Death1", "Death2", "Death3"};


ACharacterBase::ACharacterBase() :
    m_WalkSpeed(200.0f),
    m_RunSpeed(400.0f),
	m_CurSpeed(0.0f),
	m_bIsSuperArmor(false),
	m_bIsDead(false),
	m_CrowdControlTime(1.0f),
	m_OnHitPlayRate(0.01f)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECrowdControlType"), true);
	if (enumPtr != nullptr)
	{
		for (int i = 0; i < enumPtr->NumEnums(); ++i)
		{
			ECrowdControlType state = static_cast<ECrowdControlType>(enumPtr->GetValueByIndex(i));
			FOnCrowdControl_Start_Delegate startDelegate;
			m_CrowdControlStartDelegates.Add(state,startDelegate);
		}
	}
	
	m_StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	m_StatComponent->OnHPIsZero.AddUObject(this, &ACharacterBase::ExecEvent_OnHPIsZero);
	m_StatComponent->OnStaminaIsZero.AddUObject(this, &ACharacterBase::ExecEvent_OnStaminaIsZero);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	attackCount = 0;
	
	m_AnimInstanceBase = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	m_AnimInstanceBase->End_Death.AddUObject(this, &ACharacterBase::OnCalledNotify_End_Death); 
	
	m_CrowdControlStartDelegates[ECrowdControlType::Knockback].AddUObject(this, &ACharacterBase::ExecEvent_TakeKnockbackAttack);
	m_CrowdControlStartDelegates[ECrowdControlType::Airborne].AddUObject(this, &ACharacterBase::ExecEvent_TakeAirborneAttack);
	m_CrowdControlStartDelegates[ECrowdControlType::Down].AddUObject(this, &ACharacterBase::ExecEvent_TakeDownAttack);
	
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
	// CC시간 계산.
	m_CrowdControlTime = m_StatComponent->GetHitRecovery() * attackInfo->crowdControlTime;

	const float finalDamage = m_StatComponent->OnDamageHP(damage); // 실제 체력에 반영 및, 실제 깎인 체력값 리턴.(방어력을 반영한)
	
	// 피격정보 생성.
	const FHitInformation hitInfo = { attackInfo->attackName, this->Tags[0], this->GetActorLocation(),
		finalDamage, m_CrowdControlTime, bIsCriticalAttack};
	
	OnTakeDamage.Broadcast(hitInfo); // 데미지UI 및 HitEffect
	
	if (!m_bIsDead && !m_bIsSuperArmor)
	{
		if (attackInfo->bHasCrowdControl)
		{
			execEvent_CommonCrowdControl(instigator);
			
			const ECrowdControlType crowdControl = attackInfo->crowdControlType;
			m_HitDirection = Utility::GetHitDirection(instigator->GetActorLocation(), this); // 피격방향 계산.
			
			m_CrowdControlStartDelegates[crowdControl].Broadcast(instigator, attackInfo);
		
			if (attackInfo->knockBackDistance > 0.0f)
			{
				ApplyKnockback(attackInfo->knockBackDistance, instigator->GetActorLocation());
			}

			if (IsValid(m_HitSound))
			{
				UGameplayStatics::PlaySoundAtLocation(this, m_HitSound, GetActorLocation());
			}
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

void ACharacterBase::ExecEvent_TakeKnockbackAttack(AActor* instigator, const FAttackInformation* attackInfo)
{
	if (m_CurCrowdControlState == ECrowdControlStates::Down) 
	{
		CallTimer_ExecDownEvent_WhenOnGround();
	}
	else if (m_CurCrowdControlState == ECrowdControlStates::KnockbackInAir)
	{
		playOnHitMontage(TEXT("Knockback_Air"));

		DisableMovementComponentForDuration(0.2f);
		CallTimer_ExecDownEvent_WhenOnGround();
	}
	else // KnockbackOnGround
	{
		playOnHitMontage(KnockbackMontageNames[m_HitDirection]);
		SetCrowdControlState(ECrowdControlStates::KnockbackOnStanding);

		GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&ACharacterBase::OnCalledTimer_KnockbackOnStanding_End,
			m_CrowdControlTime, false);
	}

	UAnimMontage* curActivateMontage = m_AnimInstanceBase->GetCurrentActiveMontage();
	if (curActivateMontage != nullptr)
	{
		m_LastPlayedOnHitMontageName = curActivateMontage->GetFName();
	}
}

void ACharacterBase::OnCalledTimer_KnockbackOnStanding_End()
{
	if (m_bIsDead)
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		return;
	}
	
	FName curMontageName = "";
	
	if (m_AnimInstanceBase->GetCurrentActiveMontage() != nullptr)
	{
		curMontageName = m_AnimInstanceBase->GetCurrentActiveMontage()->GetFName();
	}
	
	if (curMontageName == m_LastPlayedOnHitMontageName) // 피격상태를 유지하고 있었더라면 ( 이 함수를 호출한시점에서 여전히 피격중이라면 )
	{
		m_AnimInstanceBase->StopAllMontages(0.0f); 
	}
	
	SetCrowdControlState(ECrowdControlStates::None);
}

void ACharacterBase::ExecEvent_TakeAirborneAttack(AActor* instigator,
	const FAttackInformation* attackInfo)
{
	FVector airbornePower = {0.0f, 0.0f, attackInfo->airbornePower};

	if (m_CurCrowdControlState == ECrowdControlStates::Down) 
	{
		airbornePower.Z /= 2; // 다운상태에서의 에어본은 보정값을 먹인다.
		playOnHitMontage(TEXT("Down"));
	}
	else 
	{
		playOnHitMontage(TEXT("Knockback_Air"));
		SetCrowdControlState(ECrowdControlStates::KnockbackInAir);
	}

	CallTimer_ExecDownEvent_WhenOnGround();

	FVector launchVelocity = airbornePower;
	this->LaunchCharacter(launchVelocity, true, true);
}

void ACharacterBase::ExecEvent_TakeDownAttack(AActor* instigator, const FAttackInformation* attackInfo)
{
	if (m_CurCrowdControlState == ECrowdControlStates::KnockbackInAir)
	{
		playOnHitMontage(TEXT("Knockback_Air"));
		DisableMovementComponentForDuration(0.2f); 
	}
	else
	{
		playOnHitMontage(TEXT("Down"));
		SetCrowdControlState(ECrowdControlStates::Down);

		const float downPlayTime = m_AnimInstanceBase->GetMontagePlayTime(TEXT("Down")) + 0.2f; // 다운몽타주재생시간이 CC시간보다 짧을경우를 대비한 보정값
		GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&ACharacterBase::OnCalledTimer_Down_End,
			m_CrowdControlTime > downPlayTime ? m_CrowdControlTime : downPlayTime,
			false);
	}
}

void ACharacterBase::CallTimer_ExecDownEvent_WhenOnGround()
{
	GetWorldTimerManager().SetTimer(m_CrowdControlTimerHandle,
		this,
			&ACharacterBase::ExecEvent_Down_WhenOnGround,	
			GetWorld()->DeltaTimeSeconds, true,-1);
}

void ACharacterBase::ExecEvent_Down_WhenOnGround() 
{
	if (m_bIsDead)
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		return;
	}

	if (GetCharacterMovement()->IsMovingOnGround()) 
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		
		playOnHitMontage(TEXT("Down"));
		SetCrowdControlState(ECrowdControlStates::Down);

		const float downPlayTime = m_AnimInstanceBase->GetMontagePlayTime(TEXT("Down")) + 0.2f;

		GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&ACharacterBase::OnCalledTimer_Down_End,
			m_CrowdControlTime > downPlayTime ? m_CrowdControlTime : downPlayTime,
			false);
	}
}

void ACharacterBase::OnCalledTimer_Down_End()
{
	playOnHitMontage(TEXT("GetUp"));

	const float getupPlayTime = m_AnimInstanceBase->GetMontagePlayTime(TEXT("GetUp")) + 0.2f;
	
	GetWorldTimerManager().SetTimer(
		m_CrowdControlTimerHandle,
		[this]()
		{
			SetCrowdControlState(ECrowdControlStates::None);
		},
		getupPlayTime,
		false);
}

void ACharacterBase::DisableMovementComponentForDuration(float duration) const
{
	GetCharacterMovement()->Deactivate();

	FTimerHandle activateTimer;
	GetWorldTimerManager().SetTimer(activateTimer,
		[this]()
		{ GetCharacterMovement()->Activate();},
			duration, false);
}

void ACharacterBase::ExecEvent_OnHPIsZero()
{
	Die();
}

void ACharacterBase::Die()
{
	m_bIsDead = true;
	m_StatComponent->SetCanRecoveryHP(false);
	m_StatComponent->SetCanRecoveryStamina(false);
	OnDeath.Broadcast();
	
	SetCrowdControlState(ECrowdControlStates::Dead);

	if (GetWorldTimerManager().IsTimerActive(m_CrowdControlTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
	}
	
	m_AnimInstanceBase->StopAllMontages(0.0f);
	
	UAnimMontage* deathMontage = m_AnimInstanceBase->GetMontage(DeathMontageNames[m_HitDirection]);
	if (deathMontage != nullptr)
	{
		m_AnimInstanceBase->PlayMontage(deathMontage == nullptr ? "Death0" : DeathMontageNames[m_HitDirection]);
	}

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

void ACharacterBase::playOnHitMontage(const FName& montageName)
{
	m_AnimInstanceBase->PlayMontage(montageName);
	
	UAnimMontage* curMontage = m_AnimInstanceBase->GetMontage(montageName);
	m_AnimInstanceBase->Montage_SetPlayRate(curMontage,m_OnHitPlayRate);
	
	FTimerHandle timer;
	GetWorldTimerManager().SetTimer
	( 
		timer,
		[=]()
		{
			m_AnimInstanceBase->Montage_SetPlayRate(curMontage,1.0f);
		},
		m_GameSpeedDelay,
		false);
}

void ACharacterBase::ApplyKnockback(const float knockbackDistance, const FVector& instigatorLocation)
{
	FVector dirToInstigator = instigatorLocation - this->GetActorLocation(); 
	dirToInstigator.Normalize();
	dirToInstigator *= -1 * knockbackDistance;
	dirToInstigator.Z = 0.0f;
	this->SetActorLocation(GetActorLocation() + dirToInstigator, false);
}

void ACharacterBase::SetIsSuperArmor(const bool bIsSuperArmor)
{
	m_bIsSuperArmor = bIsSuperArmor;
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

void ACharacterBase::ClearCrowdControlTimerHandle()
{
	GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
}

void ACharacterBase::SetInvincible(const bool bIsInvincible)
{
	UShapeComponent* hitCollider = GetCollider(TEXT("HitCollider"));

	if (hitCollider != nullptr)
	{
		if (bIsInvincible)
		{
			hitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			hitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

UShapeComponent* ACharacterBase::GetCollider(const FName& colliderName) const
{
	return m_Colliders.Contains(colliderName) ? m_Colliders[colliderName].Get() : nullptr;
}
