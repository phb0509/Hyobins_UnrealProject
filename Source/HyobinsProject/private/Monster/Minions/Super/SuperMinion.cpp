// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAnim.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Utility/EnumTypes.h"
#include "Utility/CustomStructs.h"
#include "Utility/Utility.h"


int32 ASuperMinion::TagCount(0);
const FName ASuperMinion::HitColliderName = "HitCollider";
const FName ASuperMinion::LeftSwordColliderName = "LeftSwordCollider";
const FName ASuperMinion::RightSwordColliderName = "RightSwordCollider";
const FName ASuperMinion::KnockbackMontageNames[4] = {"Knockback0", "Knockback1", "Knockback2", "Knockback3"};
const FName ASuperMinion::DeathMontageNames[2] = {"Death0", "Death1"};


ASuperMinion::ASuperMinion()
{
	PrimaryActorTick.bCanEverTick = true; // 로그출력용.
	AIControllerClass = ASuperMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	Tags.Add(FName("SuperMinion" + FString::FromInt(++TagCount)));
	
	initAssets();
}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInstance = Cast<USuperMinionAnim>(GetMesh()->GetAnimInstance());
	m_AIController = Cast<ASuperMinionAIController>(GetController());

	bindFuncOnMontagEvent();
}

void ASuperMinion::OnCalled_NormalAttack_End()
{
	if (m_CurFSMState == ESuperMinionFSMStates::NormalAttack)
	{
		m_AIController->StartBehaviorTree();
	}
}

void ASuperMinion::ExecEvent_TakeKnockbackAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo)
{
	Super::ExecEvent_TakeKnockbackAttack(instigator, attackInfo);
	
	if (!m_bIsSuperArmor)
	{
		if (m_CurCrowdControlState == ECrowdControlStates::Down) // 다운상태에서 피격시,
		{
			CallTimer_ExecDownEvent_WhenOnGround();
		}
		else if (m_CurCrowdControlState == ECrowdControlStates::KnockbackInAir) // 공중넉백상태에서 넉백공격 피격시,
		{
			m_AnimInstance->PlayMontage(TEXT("Knockback_Air"));
			
			DisableMovementForDuration(0.2f);
			CallTimer_ExecDownEvent_WhenOnGround();
		}
		else // 스탠딩상태 or 그 외의 FSM상태일 때
		{
			m_AnimInstance->PlayMontage(KnockbackMontageNames[m_HitDirection],1.0f);
			SetCrowdControlState(ECrowdControlStates::KnockbackOnStanding);
			
			GetWorldTimerManager().SetTimer(m_CrowdControlTimerHandle,
				this,
					&ASuperMinion::OnCalledTimer_KnockbackOnStanding_End,	
					m_CrowdControlTime, false);
		}
	}
}

void ASuperMinion::OnCalledTimer_KnockbackOnStanding_End() // 임의로 지정한 넉백시간 끝날 때 호출.
{
	if (m_bIsDead)
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		return;
	}
	
	SetCrowdControlState(ECrowdControlStates::None);
	m_AnimInstance->StopAllMontages(0.0f);
}

void ASuperMinion::ExecEvent_TakeAirborneAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo) // 띄우기공격 당하면,
{
	Super::ExecEvent_TakeAirborneAttack(instigator, attackInfo);
	
	if (!m_bIsSuperArmor)
	{
		FVector airbornePower = {0.0f, 0.0f, attackInfo->airbornePower};
		
		if (m_CurCrowdControlState == ECrowdControlStates::Down) // 다운상태에서 에어본공격맞으면, 모션만 재생한다. 조금 덜띄운다.
		{
			airbornePower.Z /= 2; // 다운상태라서 조금 덜띄운다.
			m_AnimInstance->PlayMontage(TEXT("Down"));
		}
		else // 공중넉백상태거나, 스탠딩상태거나. 타이머 호출.
		{
			m_AnimInstance->PlayMontage(TEXT("Knockback_Air"));
			SetCrowdControlState(ECrowdControlStates::KnockbackInAir);
		}
		
		CallTimer_ExecDownEvent_WhenOnGround();
		
		FVector LaunchVelocity = airbornePower; 
		this->LaunchCharacter(LaunchVelocity, true, true);
	}
}

void ASuperMinion::CallTimer_ExecDownEvent_WhenOnGround()
{
	GetWorldTimerManager().SetTimer(m_CrowdControlTimerHandle,
		this,
			&ASuperMinion::ExecEvent_Down_WhenOnGround,	
			GetWorld()->DeltaTimeSeconds, true,-1);
}

void ASuperMinion::ExecEvent_Down_WhenOnGround()
{
	Super::ExecEvent_Down_WhenOnGround();
	
	if (m_bIsDead)
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		return;
	}
	
	if (GetCharacterMovement()->IsMovingOnGround()) // 땅에 닿으면
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		
		m_AnimInstance->PlayMontage(TEXT("Down"));
		SetCrowdControlState(ECrowdControlStates::Down);
			
		const float downPlayTime = m_AnimInstance->GetMontagePlayTime(TEXT("Down")) + 0.2f;
			
		GetWorldTimerManager().SetTimer(m_CrowdControlTimerHandle,
			this,
				&ASuperMinion::OnCalledTimer_Down_End,	
				m_CrowdControlTime > downPlayTime ? m_CrowdControlTime : downPlayTime,
				false);
	}
}

void ASuperMinion::ExecEvent_TakeDownAttack(const ACharacterBase* instigator, const FAttackInformation* attackInfo)
{
	Super::ExecEvent_TakeDownAttack(instigator, attackInfo);

	if (!m_bIsSuperArmor)
	{
		if (m_CurCrowdControlState == ECrowdControlStates::KnockbackInAir) // 공중넉백상태에서 다운공격맞더라도 공중넉백유지. 그냥 다운공격아니라 넉백공격했다는 판정.
		{
			m_AnimInstance->PlayMontage(TEXT("Knockback_Air"));

			DisableMovementForDuration(0.2f);
		}
		else // 스탠딩, 다운상태이거나 그 외의상태(공격도중, 순찰 등)일 때, 다운시키기.
		{
			m_AnimInstance->PlayMontage(TEXT("Down"));
			SetCrowdControlState(ECrowdControlStates::Down);
			
			const float montagePlayTime = m_AnimInstance->GetMontagePlayTime(TEXT("Down")) + 0.2f;
			GetWorldTimerManager().SetTimer(m_CrowdControlTimerHandle,
				this,
					&ASuperMinion::OnCalledTimer_Down_End,	
					m_CrowdControlTime > montagePlayTime ? m_CrowdControlTime : montagePlayTime,
					false);
		}
	}
}

void ASuperMinion::OnCalledTimer_Down_End()
{
	Super::OnCalledTimer_Down_End();

	m_AnimInstance->PlayMontage(TEXT("GetUp"));

	const float getupPlayTime = m_AnimInstance->GetMontagePlayTime(TEXT("GetUp")) + 0.2f;
	GetWorldTimerManager().SetTimer(m_CrowdControlTimerHandle,
        		[this]()
        		{
        			SetCrowdControlState(ECrowdControlStates::None);
        		},
        	getupPlayTime,
        	false);

}

void ASuperMinion::Die()
{
	GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);

	GetCharacterMovement()->Activate();
	SetCrowdControlState(ECrowdControlStates::Dead);
	
	m_AIController->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, nullptr);
	m_AIController->StopBehaviorTree();
	m_AnimInstance->StopAllMontages(0.0f);  
	m_AnimInstance->PlayMontage(DeathMontageNames[m_HitDirection <= 1 ? 0 : 1]);
	
	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASuperMinion::ExecEvent_EndedDeathMontage() // 사망몽타주 재생완료 후 호출.		
{
	m_DeathTimeline.Play();
}

void ASuperMinion::OnCalledTimelineEvent_Loop_AfterDeath(float curveValue)
{
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatioOnDeath"), 1-(curveValue*2));
}

void ASuperMinion::OnCalledTimelineEvent_End_AfterDeath()
{
	DeActivate();
	m_DeathTimeline.SetNewTime(0.0f);
}

void ASuperMinion::DisableMovementForDuration(float duration) const
{
	GetCharacterMovement()->Deactivate();

	FTimerHandle activateTimer;
	GetWorldTimerManager().SetTimer(activateTimer,
		[this]()
		{ GetCharacterMovement()->Activate();},
			duration, false); // 넉백시간만큼하면 너무 길어서 0.2f정도로
}

void ASuperMinion::Activate()
{
	Super::Activate();
	
	SetFSMState(static_cast<uint8>(ESuperMinionFSMStates::Patrol));
	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASuperMinion::DeActivate()
{
	Super::DeActivate();

	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASuperMinion::bindFuncOnMontagEvent()
{
	if (m_AnimInstance != nullptr)
	{
		// NormalAttack
		m_AnimInstance->BindFunc_OnMontageNotInterruptedEnded(TEXT("NormalAttack0"), this,TEXT("OnCalled_NormalAttack_End"));
		m_AnimInstance->BindFunc_OnMontageNotInterruptedEnded(TEXT("NormalAttack1"), this,TEXT("OnCalled_NormalAttack_End"));

		m_AnimInstance->BindFunc_OnMontageInterruptedEnded(TEXT("NormalAttack0"), this,TEXT("OnCalled_NormalAttack_End"));
		m_AnimInstance->BindFunc_OnMontageInterruptedEnded(TEXT("NormalAttack1"), this,TEXT("OnCalled_NormalAttack_End"));
	}

	if (m_DeathCurveFloat != nullptr)
	{
		m_DeathTimeline.SetLooping(false);
		
		FOnTimelineFloat afterDeathTimeline_Loop;
		afterDeathTimeline_Loop.BindDynamic(this, &ASuperMinion::OnCalledTimelineEvent_Loop_AfterDeath);
		m_DeathTimeline.AddInterpFloat(m_DeathCurveFloat, afterDeathTimeline_Loop);

		FOnTimelineEvent afterDeathTimeline_End;
		afterDeathTimeline_End.BindDynamic(this, &ASuperMinion::OnCalledTimelineEvent_End_AfterDeath);
		m_DeathTimeline.SetTimelineFinishedFunc(afterDeathTimeline_End);
	}
}

void ASuperMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	m_DeathTimeline.TickTimeline(DeltaTime);
	
	const FString movementMode = GetCharacterMovement()->GetMovementName();
	FString log = TEXT("SuperMinion Mode :: ");
	log += movementMode;
	GEngine->AddOnScreenDebugMessage(100, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log));
	
	
	FString fsmState = Utility::ConvertEnumToString(static_cast<ESuperMinionFSMStates>(m_CurFSMState));
	FString log1 = Tags[0].ToString() + " :: " + fsmState;
	GEngine->AddOnScreenDebugMessage(104, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log1));

	FString crowdState = Utility::ConvertEnumToString(m_CurCrowdControlState);
    FString log2 = Tags[0].ToString() + " :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(105, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log2));
}

void ASuperMinion::initAssets()
{
	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/MonsterAsset/SuperMinion/Character/SuperMinion.SuperMinion'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	checkf(IsValid(mesh.Object), TEXT("Mesh is not Valid"));

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstance(TEXT("AnimBlueprint'/Game/MonsterAsset/SuperMinion/ABP_SuperMinion.ABP_SuperMinion_C'"));
	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
	checkf(IsValid(animInstance.Class), TEXT("AnimInstance is not Valid"));
	
	// HitCollider
	m_HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	m_HitCollider->SetupAttachment(GetMesh(), FName(TEXT("spine_01")));
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("HitCollider_Monster")); 
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_HitCollider->SetNotifyRigidBodyCollision(false);
	m_HitCollider->SetGenerateOverlapEvents(true);
	
	// rotation.y(pitch), rotation.z(yaw), rotation.x(roll)
	// location.x, location.y, location. z
	// scale.x,scale.y,scale.z
	
	FTransform collisionTransform = {
		{0.0f, 0.0f, 20.0f},
		{-11.111f, 115.6134, 10.4},
		{0.375f, 1.5f, 0.625f} };
	// LeftSwordCollider
	m_LeftSwordCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSwordCollider"));
	m_LeftSwordCollider->SetWorldTransform(collisionTransform);
	m_LeftSwordCollider->SetupAttachment(GetMesh(), FName(TEXT("weapon_l")));
	m_LeftSwordCollider->SetCollisionProfileName(TEXT("AttackCollider_Monster")); 
	m_LeftSwordCollider->SetGenerateOverlapEvents(true);
	m_LeftSwordCollider->SetNotifyRigidBodyCollision(false);
	m_LeftSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	collisionTransform = {
		{0.0f, 0.0f, 160.0f},
		{0.0f, -113.333f, -13.3333f},
		{0.25f, 1.5f, 0.625f} };
	
	// RightSwordCollider
	m_RightSwordCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSwordCollider"));
	m_RightSwordCollider->SetWorldTransform(collisionTransform);
	m_RightSwordCollider->SetupAttachment(GetMesh(), FName(TEXT("weapon_r")));
	m_RightSwordCollider->SetCollisionProfileName(TEXT("AttackCollider_Monster")); 
	m_RightSwordCollider->SetGenerateOverlapEvents(true);
	m_RightSwordCollider->SetNotifyRigidBodyCollision(false);
	m_RightSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Colliders.Add(HitColliderName, m_HitCollider);
	m_Colliders.Add(LeftSwordColliderName, m_LeftSwordCollider);
	m_Colliders.Add(RightSwordColliderName, m_RightSwordCollider);
}