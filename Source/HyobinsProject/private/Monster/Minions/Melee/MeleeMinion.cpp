// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Monster/Minions/Melee/MeleeMinionAnim.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

int AMeleeMinion::TagCount(0);

AMeleeMinion::AMeleeMinion() :
	m_CurState(ENormalMinionStates::Patrol)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMeleeMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(FName("MeleeMinion" + FString::FromInt(++TagCount)));
	m_HitRecovery = 1.0f;
	m_PatrolRange = 500.0f;

	initAssets();
}

void AMeleeMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_AnimInstance = Cast<UMeleeMinionAnim>(GetMesh()->GetAnimInstance());
	if (m_AnimInstance.IsValid())
	{
		m_AnimInstance->OnMontageEnded.AddDynamic(this, &AMeleeMinion::OnMontageEnded); // 몽타주 재생완료시 호출할 함수 바인딩.
		m_AnimInstance->OnDeathMontageEnded.AddUObject(this, &ACharacterBase::OnCalledDeathMontageEndedNotify);
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AnimInstance is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AnimInstance is not Valid"));
	}

	m_AIController = Cast<AMeleeMinionAIController>(GetController());
	if (m_AIController.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AIController is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AIController is not Valid"));
	}
}

void AMeleeMinion::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
}
void AMeleeMinion::NormalAttack()
{
	if (m_bIsAttacking) return;

	m_bIsAttacking = true;

	m_AnimInstance->PlayNormalAttackMontage();
}

void AMeleeMinion::SetState(ENormalMinionStates state)
{
	m_CurState = state;
	m_AIController->GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, static_cast<uint8>(state));
}

void AMeleeMinion::SetHitState()
{
	m_AnimInstance->Montage_Play(m_AnimInstance->GetOnHitMontages()[0]);
	SetState(ENormalMinionStates::Hit);
}

void AMeleeMinion::OnHitTimerEnded()
{
	if (m_bIsDeath)
	{
		GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
		return;
	}

	SetState(ENormalMinionStates::Patrol);
	GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
}

void AMeleeMinion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) // 현재 재생중인 몽타주의 재생이 끝났을 시 호출.
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

void AMeleeMinion::onNormalAttackMontageEnded()
{
	m_bIsAttacking = false;
}

void AMeleeMinion::Die()
{
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetState(ENormalMinionStates::Die);
	m_AnimInstance->Montage_Play(m_AnimInstance->GetDeathMontages()[0]);
	m_AIController->OnUnPossess();
}

void AMeleeMinion::initAssets()
{
	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/MonsterAsset/Minion/Character/MeleeMinion.MeleeMinion'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	checkf(IsValid(mesh.Object), TEXT("Mesh is not Valid"));

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstance(TEXT("AnimBlueprint'/Game/MonsterAsset/Minion/ABP_MeleeMinion.ABP_MeleeMinion_C'"));
	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
	checkf(IsValid(animInstance.Class), TEXT("animInstance is not Valid"));

	// HitCollider
	m_HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	m_HitCollider->SetupAttachment(RootComponent);
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("ACharacterBase"));
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void AMeleeMinion::updateState()
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