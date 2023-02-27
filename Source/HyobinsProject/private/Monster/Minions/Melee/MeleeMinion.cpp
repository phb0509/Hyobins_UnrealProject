// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Monster/Minions/Melee/MeleeMinionAIController.h"


AMeleeMinion::AMeleeMinion() 
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMeleeMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Super::LoadMesh("SkeletalMesh'/Game/MonsterAsset/Minion/Character/MeleeMinion.MeleeMinion'");
	m_Name = FName("MeleeMinion");

	m_PatrolRange = 500.0f;
	//initComponents();
	//initCollisions();
	//initAttackInformations();
}


void AMeleeMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

