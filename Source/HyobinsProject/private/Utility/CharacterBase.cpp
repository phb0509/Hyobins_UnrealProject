// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AnimInstanceBase.h"
#include "Utility/Utility.h"
#include "Utility/CustomStructs.h"
#include "Utility/EnumTypes.h"
#include "Component/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "SubSystems/DataManager.h"


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

void ACharacterBase::Attack(const FName& attackName, TWeakObjectPtr<AActor> target)
{
	if (!target.IsValid())
	{
		return;
	}
	
	IDamageable* damageableActor = nullptr;
	damageableActor = Cast<IDamageable>(target);

	if (damageableActor == nullptr)
	{
		return;
	}
	
	const UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	const FAttackInformation* attackInfo = dataManager->GetAttackInformation(this->GetClass(), attackName);
	
	const bool bIsCriticalAttack = FMath::FRandRange(0.0f, 100.0f) <= m_StatComponent->GetCriticalAttackChance();
	const float finalDamage = (m_StatComponent->GetDefaultDamage() * attackInfo->damageRatio) * (bIsCriticalAttack ? 2.0f : 1.0f);
	
	damageableActor->OnDamage(finalDamage, bIsCriticalAttack, attackInfo, this);
}

void ACharacterBase::ClearCrowdControlTimerHandle()
{
	GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
}

void ACharacterBase::OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation* attackInfo, const ACharacterBase* instigator)
{
	m_CrowdControlTime = m_StatComponent->GetHitRecovery() * attackInfo->crowdControlTime;
	const FHitInformation hitInfo = { attackInfo->attackName, this->Tags[0], this->GetActorLocation(),
		damage, m_CrowdControlTime,bIsCriticalAttack};
	
	OnTakeDamage.Broadcast(hitInfo); // 데미지UI 및 HitEffect
	m_StatComponent->OnDamageHP(damage);
	
	if (!m_bIsDead && !m_bIsSuperArmor)
	{
		if (attackInfo->bHasCrowdControl)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_HitSound, GetActorLocation());
			execEvent_CommonCrowdControl(instigator);
			
			const ECrowdControlType crowdControl = attackInfo->crowdControlType;
			m_HitDirection = Utility::GetHitDirection(this, instigator); // 피격방향 계산.
			
			m_CrowdControlStartDelegates[crowdControl].Broadcast(instigator, attackInfo);
		
			if (attackInfo->knockBackDistance > 0.0f)
			{
				FVector dirToInstigator = instigator->GetActorLocation() - this->GetActorLocation(); 
				dirToInstigator.Normalize();
				dirToInstigator *= -1 * attackInfo->knockBackDistance;
				dirToInstigator.Z = 0.0f;
				this->SetActorLocation(GetActorLocation() + dirToInstigator, false);
			}
		}
	}
	
	// 로그용
	++attackCount;
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(damage) + " damage from " +
		instigator->Tags[0].ToString() + "::" + attackInfo->attackName.ToString() + "::" + FString::FromInt(attackCount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
}

void ACharacterBase::ExecEvent_TakeKnockbackAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo)
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
	else 
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

void ACharacterBase::ExecEvent_TakeAirborneAttack(const ACharacterBase* instigator,
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

void ACharacterBase::ExecEvent_TakeDownAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo)
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
	
	GetCharacterMovement()->Activate();
	
	m_AnimInstanceBase->StopAllMontages(0.0f);

	UAnimMontage* deathMontage = m_AnimInstanceBase->GetMontage(DeathMontageNames[m_HitDirection]);
	m_AnimInstanceBase->PlayMontage(deathMontage == nullptr ? "Death0" : DeathMontageNames[m_HitDirection]);
	
	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ACharacterBase::RotateToTarget(const AActor* target, const FRotator& rotatorOffset)
{
	const FVector targetLocation = target->GetActorLocation();
    const FVector curLocation = GetActorLocation();
	
    const FVector directionToTarget = (targetLocation - curLocation).GetSafeNormal();
    const FRotator rotationToTarget = directionToTarget.Rotation();
	
	FRotator curRotation = GetActorRotation();
	curRotation.Yaw = rotationToTarget.Yaw;
	curRotation += rotatorOffset;
	
    SetActorRotation(curRotation);
}

