// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"

// Sets default values
AMonster::AMonster() :
	m_PatrolRange(3000.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	//m_BehaviorTree->BlackboardAsset;
}





