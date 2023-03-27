// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "Monster/Minions/Melee/MeleeMinionAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

int AMeleeMinion::TagCount(0);

AMeleeMinion::AMeleeMinion() :
	m_CurState(ENormalMinionStates::Patrol),
	m_NextState(ENormalMinionStates::Patrol)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMeleeMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(FName("MeleeMinion" + FString::FromInt(++TagCount)));

	Super::LoadMesh("SkeletalMesh'/Game/MonsterAsset/Minion/Character/MeleeMinion.MeleeMinion'");
	Super::LoadAnimInstance("AnimBlueprint'/Game/MonsterAsset/Minion/ABP_MeleeMinion.ABP_MeleeMinion_C'");

	m_HitRecovery = 2.0f;
	m_PatrolRange = 500.0f;

	m_HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	m_HitCollider->SetupAttachment(RootComponent);
	//m_HitCollider->SetWorldTransform(collisionTransform);
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("ACharacterBase"));
	//m_HitCollider->SetCollisionObjectType(ECollisionChannel::);
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 필요할때만 키기

	//initComponents();
	//initCollisions();
	//initAttackInformations();
	
}

void AMeleeMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMeleeMinion::SetStateToNextState(FString state)
{
	SetState(ENormalMinionStates::Patrol);
	UE_LOG(LogTemp, Warning, TEXT("VirtualFunction is Caleed!"));
}

void AMeleeMinion::BeginPlay()
{
	Super::BeginPlay();

	m_ABPAnimInstance = Cast<UMeleeMinionAnim>(GetMesh()->GetAnimInstance());
	//m_AIController = Cast<AMeleeMinionAIController>(GetController());

	if (m_ABPAnimInstance.IsValid())
	{
		m_ABPAnimInstance->OnMontageEnded.AddDynamic(this, &AMeleeMinion::OnMontageEnded); // 공격몽타주 재생완료시 호출할 함수 바인딩.
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AnimInstance is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AnimInstance is not Valid"));
	}
}

float AMeleeMinion::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACharacterBase* instigatorCharacter = Cast<ACharacterBase>(EventInstigator->GetPawn());
	
	const FAttackInfoStruct* attackInformation = static_cast<const FAttackInfoStruct*>(&DamageEvent);
	
	m_ABPAnimInstance->PlayOnHitMontage();
	SetState(ENormalMinionStates::Hit);

	AMeleeMinionAIController* owernAIController = Cast<AMeleeMinionAIController>(GetController());
	owernAIController->GetBlackboardComponent()->SetValueAsFloat(AMonster::HitRecoveryKey, m_HitRecovery * attackInformation->knockBackTime);

	m_bIsAttacking = false; // 슈퍼아머같은 상태가 아니면 피격시 강제 온힛상태가 되니까 attacking을 false로 해줘야 한다.

	// 로그.
	FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	return FinalDamage;
}

void AMeleeMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
}
void AMeleeMinion::NormalAttack()
{
	FString temp;

	if (m_bIsAttacking) temp = "Attacking!!!";
	else temp = "Not Attacking!!";

	UE_LOG(LogTemp, Warning, TEXT("%s"), *temp);

	if (m_bIsAttacking) return;

	m_bIsAttacking = true;

	m_ABPAnimInstance->PlayNormalAttackMontage();
}

void AMeleeMinion::SetState(ENormalMinionStates state)
{
	m_CurState = state; 
	AMeleeMinionAIController* owernAIController = Cast<AMeleeMinionAIController>(GetController());
	owernAIController->GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, static_cast<uint8>(state));
}

void AMeleeMinion::initComponents()
{
	initCollisions();
}

void AMeleeMinion::initCollisions()
{
}

void AMeleeMinion::initAttackInformations()
{
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

void AMeleeMinion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) // 기본공격몽타주가 끝까지 재생 되었을 경우 호출.
{
	int curState = (uint8)m_CurState;

	switch (curState)
	{
	case (uint8)ENormalMinionStates::Attack:
		onNormalAttackMontageEnded();
		break;
	case (uint8)ENormalMinionStates::Hit:
		onHitMontageEnded();
		break;
	default:
		break;
	}
}

void AMeleeMinion::onNormalAttackMontageEnded()
{
	m_bIsAttacking = false;
}

void AMeleeMinion::onHitMontageEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHitMontageEnde!!"));
}
