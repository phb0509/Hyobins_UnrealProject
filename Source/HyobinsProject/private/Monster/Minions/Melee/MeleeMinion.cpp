// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include <GameFramework/CharacterMovementComponent.h>

int AMeleeMinion::tagCount(0);

AMeleeMinion::AMeleeMinion() :
	m_CurState(ENormalMinionStates::Patrol)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMeleeMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Super::LoadMesh("SkeletalMesh'/Game/MonsterAsset/Minion/Character/MeleeMinion.MeleeMinion'");
	Super::LoadAnimInstance("AnimBlueprint'/Game/MonsterAsset/Minion/ABP_MeleeMinion.ABP_MeleeMinion_C'");

	m_Name = "MeleeMinion" + FString::FromInt(++tagCount);

	m_PatrolRange = 500.0f;
	//initComponents();
	//initCollisions();
	//initAttackInformations();
}


void AMeleeMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
}

void AMeleeMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMeleeMinion::BeginPlay()
{
	Super::BeginPlay();
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

