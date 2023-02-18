// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Utility/AIControllerBase.h"

AMeleeMinion::AMeleeMinion()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AAIControllerBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Super::LoadMesh("SkeletalMesh'/Game/MonsterAsset/Minion/Character/MeleeMinion.MeleeMinion'");
	m_Name = FName("MeleeMinion");

	//initComponents();
	//initCollisions();
	//initAttackInformations();
}


void AMeleeMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector curLocation = GetActorLocation();
	//SetActorLocation(FVector(curLocation.X + 10.0f, 0.0f, 0.0f));
	
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

