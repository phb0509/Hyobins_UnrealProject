// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"

// Sets default values
AMonster::AMonster() :
	m_PatrolRange(1000.0f),
	m_NormalAttackRange(30.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}





