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


int32 attackCount = 0; // 로그확인용.
const FName ACharacterBase::HitColliderName = "HitCollider";
const FName ACharacterBase::KnockbackMontageNames[4] = {"Knockback0", "Knockback1", "Knockback2", "Knockback3"};
const FName ACharacterBase::DeathMontageNames[4] = {"Death0", "Death1", "Death2", "Death3"};

ACharacterBase::ACharacterBase() :
    m_WalkSpeed(200.0f),
    m_RunSpeed(400.0f),
	m_CurSpeed(0.0f),
	m_bIsSuperArmor(false),
	m_bIsDead(false),
	m_CrowdControlTime(1.0f)
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
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	attackCount = 0;
	
	m_AnimInstanceBase = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	m_AnimInstanceBase->End_Death.AddUObject(this, &ACharacterBase::OnCalledNotify_End_Death); // Death는 Ended를 호출하지않기에, 노티파이 심어야함.
	
	m_CrowdControlStartDelegates[ECrowdControlType::Knockback].AddUObject(this, &ACharacterBase::ExecEvent_TakeKnockbackAttack);
	m_CrowdControlStartDelegates[ECrowdControlType::Airborne].AddUObject(this, &ACharacterBase::ExecEvent_TakeAirborneAttack);
	m_CrowdControlStartDelegates[ECrowdControlType::Down].AddUObject(this, &ACharacterBase::ExecEvent_TakeDownAttack);
}

void ACharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_CurSpeed = GetVelocity().Size();
	m_bIsOnGround = GetCharacterMovement()->IsMovingOnGround();
	m_bIsFalling = GetCharacterMovement()->IsFalling();
	m_bIsFlying = GetCharacterMovement()->IsFlying();
}

void ACharacterBase::Attack(const FName& attackName, TWeakObjectPtr<AActor> target) const
{
	if (!target.IsValid())
	{
		return;
	}

	ACharacterBase* targetActor = Cast<ACharacterBase>(target);
	const UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	const FAttackInformation* attackInfo = dataManager->GetAttackInformation(this->GetClass(), attackName);
	
	const bool bIsCriticalAttack = FMath::FRandRange(0.0f, 100.0f) <= m_StatComponent->GetCriticalAttackChance();
	const float finalDamage = (m_StatComponent->GetDefaultDamage() * attackInfo->damageRatio) * (bIsCriticalAttack ? 2.0f : 1.0f);
	
	targetActor->OnDamage(finalDamage, bIsCriticalAttack, attackInfo, this);
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
	
	OnTakeDamage.Broadcast(hitInfo); // 데미지 UI, 붉은색 HitEffect
	m_StatComponent->UpdateHP(damage);
	
	if (!m_bIsDead && !m_bIsSuperArmor)
	{
		if (attackInfo->bHasCrowdControl)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_HitSound, GetActorLocation());
			execEvent_CommonCrowdControl(instigator);
			
			const ECrowdControlType crowdControl = attackInfo->crowdControlType;
			m_HitDirection = Utility::GetHitDirection(this, instigator); // 피격방향을 산출.
			
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

	// 로그
	++attackCount;
	const FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(damage) + " damage from " +
		instigator->Tags[0].ToString() + "::" + attackInfo->attackName.ToString() + "::" + FString::FromInt(attackCount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
}

void ACharacterBase::ExecEvent_TakeKnockbackAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo)
{
	if (m_CurCrowdControlState == ECrowdControlStates::Down) // 다운상태에서 피격시,
	{
		CallTimer_ExecDownEvent_WhenOnGround();
	}
	else if (m_CurCrowdControlState == ECrowdControlStates::KnockbackInAir) // 공중넉백상태에서 넉백공격 피격시,
	{
		m_AnimInstanceBase->PlayMontage(TEXT("Knockback_Air"));

		DisableMovementComponentForDuration(0.2f);
		CallTimer_ExecDownEvent_WhenOnGround();
	}
	else // 스탠딩상태 or 그 외의 FSM상태일 때
	{
		m_AnimInstanceBase->PlayMontage(KnockbackMontageNames[m_HitDirection], 1.0f);
		SetCrowdControlState(ECrowdControlStates::KnockbackOnStanding);

		GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&ACharacterBase::OnCalledTimer_KnockbackOnStanding_End,
			m_CrowdControlTime, false);
	}

	m_LastPlayedOnHitMontageName = m_AnimInstanceBase->GetCurrentActiveMontage()->GetFName();
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
	
	if (curMontageName == m_LastPlayedOnHitMontageName) // 이 함수를 호출했던 피격모션을 계속 유지해야한다는 조건.
	{
		m_AnimInstanceBase->StopAllMontages(0.0f); // 스탑안하면 몬스터가 피격모션안끝난상태로 플레이어 쫓아옴.
	}
	
	SetCrowdControlState(ECrowdControlStates::None);
}

void ACharacterBase::ExecEvent_TakeAirborneAttack(const ACharacterBase* instigator,
	const FAttackInformation* attackInfo)
{
	FVector airbornePower = {0.0f, 0.0f, attackInfo->airbornePower};

	if (m_CurCrowdControlState == ECrowdControlStates::Down) // 다운상태에서 에어본공격맞으면 다운 모션 똑같이 진행.
	{
		airbornePower.Z /= 2; // 다운상태라서 조금 덜 띄운다.
		m_AnimInstanceBase->PlayMontage(TEXT("Down"));
	}
	else // 공중넉백상태거나, 스탠딩상태거나. 타이머 호출.
	{
		m_AnimInstanceBase->PlayMontage(TEXT("Knockback_Air"));
		SetCrowdControlState(ECrowdControlStates::KnockbackInAir);
	}

	CallTimer_ExecDownEvent_WhenOnGround();

	FVector LaunchVelocity = airbornePower;
	this->LaunchCharacter(LaunchVelocity, true, true);
}

void ACharacterBase::ExecEvent_TakeDownAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo)
{
	if (m_CurCrowdControlState == ECrowdControlStates::KnockbackInAir)// 공중넉백상태에서 다운공격맞더라도 공중넉백유지. 그냥 다운공격아니라 넉백공격했다는 판정.
	{
		m_AnimInstanceBase->PlayMontage(TEXT("Knockback_Air"));

		DisableMovementComponentForDuration(0.2f);
	}
	else // 스탠딩, 다운상태이거나 그 외의상태(공격도중, 순찰 등)일 때, 다운시키기.
	{
		m_AnimInstanceBase->PlayMontage(TEXT("Down"));
		SetCrowdControlState(ECrowdControlStates::Down);

		const float montagePlayTime = m_AnimInstanceBase->GetMontagePlayTime(TEXT("Down")) + 0.2f;
		GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&ACharacterBase::OnCalledTimer_Down_End,
			m_CrowdControlTime > montagePlayTime ? m_CrowdControlTime : montagePlayTime,
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

void ACharacterBase::ExecEvent_Down_WhenOnGround() // (에어본 상태에서 호출되었을 테니) 땅인지 체크 후, Down진행 -> 이후 GetUp진행.
{
	if (m_bIsDead)
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		return;
	}

	if (GetCharacterMovement()->IsMovingOnGround()) // 땅에 닿으면
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);

		m_AnimInstanceBase->PlayMontage(TEXT("Down"));
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
	m_AnimInstanceBase->PlayMontage(TEXT("GetUp"));

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
			duration, false); // 넉백시간만큼하면 너무 길어서 0.2f정도로
}

void ACharacterBase::ExecEvent_OnHPIsZero()
{
	Die();
}

void ACharacterBase::Die()
{
	m_bIsDead = true;
	OnDeath.Broadcast();
	SetCrowdControlState(ECrowdControlStates::Dead);
	GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
	GetCharacterMovement()->Activate();
	
	m_AnimInstanceBase->StopAllMontages(0.0f);  
	m_AnimInstanceBase->PlayMontage(DeathMontageNames[m_HitDirection]);
	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacterBase::OnCalledNotify_End_Death() 
{
	ExecEvent_EndedDeathMontage();
}

void ACharacterBase::RotateToTarget(const AActor* target)
{
	FRotator actorRotation = GetActorRotation();
	actorRotation.Yaw = target->GetActorRotation().Yaw;
	
	SetActorRotation(actorRotation);
}

