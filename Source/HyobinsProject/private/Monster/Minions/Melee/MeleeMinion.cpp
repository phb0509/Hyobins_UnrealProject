// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "Monster/Minions/Melee/MeleeMinionAnim.h"
#include <GameFramework/CharacterMovementComponent.h>

int AMeleeMinion::TagCount(0);

AMeleeMinion::AMeleeMinion() :
	m_CurState(ENormalMinionStates::Patrol)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMeleeMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Super::LoadMesh("SkeletalMesh'/Game/MonsterAsset/Minion/Character/MeleeMinion.MeleeMinion'");
	Super::LoadAnimInstance("AnimBlueprint'/Game/MonsterAsset/Minion/ABP_MeleeMinion.ABP_MeleeMinion_C'");

	m_Name = "MeleeMinion" + FString::FromInt(++TagCount);

	m_PatrolRange = 500.0f;
	//initComponents();
	//initCollisions();
	//initAttackInformations();
}

void AMeleeMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMeleeMinion::BeginPlay()
{
	Super::BeginPlay();

	m_ABPAnimInstance = Cast<UMeleeMinionAnim>(GetMesh()->GetAnimInstance());

	if (m_ABPAnimInstance.IsValid())
	{
		m_ABPAnimInstance->OnMontageEnded.AddDynamic(this, &AMeleeMinion::OnNormalAttackMontageEnded); // ���ݸ�Ÿ�� ����Ϸ�� ȣ���� �Լ� ���ε�.
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AnimInstance is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeMinion AnimInstance is not Valid"));
	}
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

	m_ABPAnimInstance->PlayNormalAttackMontage();
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

void AMeleeMinion::OnNormalAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) // �⺻���ݸ�Ÿ�ְ� ������ ��� �Ǿ��ų�, ���� ���� Ű�Է��� ���̻� ���ų�
{
	m_bIsAttacking = false;
	//updateNormalAttackStateOnEnd();
}
