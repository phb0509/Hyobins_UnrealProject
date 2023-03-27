// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"


const FName AMonster::HomePosKey(TEXT("HomePos"));
const FName AMonster::PatrolPosKey(TEXT("PatrolPos"));
const FName AMonster::EnemyKey(TEXT("Enemy"));
const FName AMonster::HitRecoveryKey(TEXT("HitRecovery"));
const FName AMonster::StateKey(TEXT("State"));
const FName AMonster::NextStateKey(TEXT("NextState"));


AMonster::AMonster() :
	m_PatrolRange(1000.0f),
	m_NormalAttackRange(150.0f),
	m_bIsAttacking(false)
{
	
}





