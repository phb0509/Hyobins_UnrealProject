// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"


AMeleeMinion::AMeleeMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	//Super::LoadMesh("SkeletalMesh'/Game/MainPlayerAsset/Character/MainPlayer.MainPlayer'");
	Super::LoadMesh("SkeletalMesh'/Game/MonsterAsset/Minion/Character/MeleeMinion.MeleeMinion'");
	initComponents();
	initCollisions();
	initAttackInformations();
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

