// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAnim.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
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

	InitHP(100.0f);
	m_NormalAttackSpeed = 1.0f;
	m_HitRecovery = 1.0f;
	m_PatrolRange = 500.0f;
	m_DeathTimerTime = 3.0f;

	initAssets();
}

void ASuperMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_AnimInstance = Cast<USuperMinionAnim>(m_AnimInstanceBase);
	if (m_AnimInstance.IsValid())
	{
		m_AnimInstance->OnMontageEnded.AddDynamic(this, &ASuperMinion::OnMontageEnded); 
		m_AnimInstance->OnDeathMontageEnded.AddUObject(this, &ACharacterBase::OnCalledDeathMontageEndedNotify);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SuperMinion AnimInstance is not Valid"));
	}

	m_OwnerAIController = Cast<ASuperMinionAIController>(m_AIControllerBase);
	if (m_OwnerAIController.IsValid())
	{}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SuperMinion AIController is not Valid"));
	}
}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();
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

	m_AnimInstance->PlayMontage("NormalAttack1");
	m_OwnerAIController->StopBehaviorTree();
}

void ASuperMinion::ExecDeathEvent()
{
	m_DeathTimerRemainingTime = m_DeathTimerTime;
	m_DeathTimerTickTime = m_DeathTimerTime / 100;
	GetWorld()->GetTimerManager().SetTimer(m_DeathTimerHandle, this, &ASuperMinion::OnDeathEventTimerEnded, m_DeathTimerTickTime, true, 0.0f);
}

void ASuperMinion::OnDeathEventTimerEnded()
{
	m_DeathTimerRemainingTime -= m_DeathTimerTickTime;
	m_DiffuseRatio -= m_DeathTimerTickTime;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), m_DiffuseRatio);

	if (m_DeathTimerRemainingTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_DeathTimerHandle);
	}
}

void ASuperMinion::OnHitTimerEnded()
{
	if (m_bIsDeath)
	{
		GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
		return;
	}

	SetState(ENormalMinionStates::Chase);
	GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
}

void ASuperMinion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) 
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
	m_OwnerAIController->PlayBehaviorTree();
}

void ASuperMinion::Die()
{
	SetState(ENormalMinionStates::Die);

	m_HitColliders[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//m_AnimInstance->Montage_Play(m_AnimInstance->GetDeathMontages()[TagCount % 2]);
	m_AnimInstance->PlayMontage("OnDeath_Front");
	m_OwnerAIController->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, nullptr);
	m_OwnerAIController->OnUnPossess();
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
	UCapsuleComponent* hitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	hitCollider->SetupAttachment(RootComponent);
	hitCollider->SetCapsuleHalfHeight(60.0f);
	hitCollider->SetCapsuleRadius(60.0f);
	hitCollider->SetCollisionProfileName(TEXT("HitCollider")); // HitCollider ÇÁ¸®¼Â
	hitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	hitCollider->SetNotifyRigidBodyCollision(false);
	hitCollider->SetGenerateOverlapEvents(true);

	m_HitColliders.Add(hitCollider);
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

void ASuperMinion::SetState(ENormalMinionStates state)
{
	m_CurState = state;
	m_OwnerAIController->GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, static_cast<uint8>(state));
}

void ASuperMinion::SetCommonState(EMonsterCommonStates commonState)
{
	int8 index = static_cast<int8>(commonState);

	switch (index)
	{
	case static_cast<int8>(EMonsterCommonStates::Patrol):
		SetState(ENormalMinionStates::Patrol);
		break;
	case static_cast<int8>(EMonsterCommonStates::Hit):
		m_AnimInstance->PlayMontage("OnHit_Front");
		SetState(ENormalMinionStates::Hit);
		break;
	case static_cast<int8>(EMonsterCommonStates::Die):
		break;
	default:
		break;
	}
}