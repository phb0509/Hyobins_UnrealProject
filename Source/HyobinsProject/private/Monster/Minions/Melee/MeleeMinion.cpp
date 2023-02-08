// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinion.h"




AMeleeMinion::AMeleeMinion()
{
	initComponents();
	loadMesh();
	loadAnimInstance();
	initCollisions();
	initAttackInformations();
}


void AMeleeMinion::initComponents()
{
	initCollisions();
}

void AMeleeMinion::loadMesh()
{
	// 메쉬 로드.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempMesh(TEXT("SkeletalMesh'/Game/MainPlayerAsset/Character/MainPlayer.MainPlayer'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
}

void AMeleeMinion::loadAnimInstance()
{
}

void AMeleeMinion::initCollisions()
{
}

void AMeleeMinion::initAttackInformations()
{
}

