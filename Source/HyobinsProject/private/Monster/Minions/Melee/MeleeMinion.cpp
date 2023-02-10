// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"


AMeleeMinion::AMeleeMinion()
{
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

