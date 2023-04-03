// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Monster/Minions/Super/SuperMinionAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

int ASuperMinion::TagCount(0);

ASuperMinion::ASuperMinion() :
	m_CurState(ENormalMinionStates::Patrol)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ASuperMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(FName("SuperMinion" + FString::FromInt(++TagCount)));

	Super::LoadMesh("SkeletalMesh'/Game/MonsterAsset/SuperMinion/Character/SuperMinion.SuperMinion'");
	Super::LoadAnimInstance("AnimBlueprint'/Game/MonsterAsset/SuperMinion/ABP_SuperMinion.ABP_SuperMinion_C'");
	UE_LOG(LogTemp, Warning, TEXT("This is SuperMinionClass"));

	m_HitRecovery = 1.0f;
	m_PatrolRange = 500.0f;

	initComponents();
}

void ASuperMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInstance = Cast<USuperMinionAnim>(GetMesh()->GetAnimInstance());

	if (m_AnimInstance.IsValid())
	{
		m_AnimInstance->OnMontageEnded.AddDynamic(this, &ASuperMinion::OnMontageEnded); // 몽타주 재생완료시 호출할 함수 바인딩.
		UE_LOG(LogTemp, Warning, TEXT("SuperMinion AnimInstance is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SuperMinion AnimInstance is not Valid"));
	}

	m_AIController = Cast<ASuperMinionAIController>(GetController());
	if (m_AIController.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SuperMinion AIController is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SuperMinion AIController is not Valid"));
	}
}

void ASuperMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
}
void ASuperMinion::NormalAttack()
{
	if (m_bIsAttacking) return;

	m_bIsAttacking = true;

	m_AnimInstance->PlayNormalAttackMontage();
}

void ASuperMinion::SetState(ENormalMinionStates state)
{
	m_CurState = state;
	m_AIController->GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, static_cast<uint8>(state));
}

void ASuperMinion::SetHitState()
{
	m_AnimInstance->Montage_Play(m_AnimInstance->GetOnHitMontages()[0]);
	SetState(ENormalMinionStates::Hit);
}

void ASuperMinion::OnHitTimerEnded()
{
	SetState(ENormalMinionStates::Patrol);
	GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
}

void ASuperMinion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) // 현재 재생중인 몽타주의 재생이 끝났을 시 호출.
{
	int curState = (uint8)m_CurState;

	switch (curState)
	{
	case (uint8)ENormalMinionStates::NormalAttack:
		onNormalAttackMontageEnded();
		break;
	default:
		break;
	}
}

void ASuperMinion::onNormalAttackMontageEnded()
{
	m_bIsAttacking = false;
}

void ASuperMinion::onHitMontageEnded()
{

}

void ASuperMinion::Die()
{
	m_bIsDeath = true;

	m_AIController->StopBehaviorTree();
	m_AnimInstance->SetDeathSequenceIndex(0);
}

void ASuperMinion::initComponents()
{
	initCollisions();
}

void ASuperMinion::initCollisions()
{
	m_HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	m_HitCollider->SetupAttachment(RootComponent);
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("ACharacterBase"));
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void ASuperMinion::updateState()
{
	m_CurSpeed = GetVelocity().Size();
	m_bIsInAir = GetMovementComponent()->IsFalling();

	if (!m_bIsInAir)
	{
		if (m_CurSpeed < 0.1f)
		{
			m_bIsIdle = true;
			m_bIsWalking = false;
		}
		else
		{
			m_bIsIdle = false;
			m_bIsWalking = true;
		}
	}
}