// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterManager.h"
#include "Monster/Monster.h"
#include "Monster/Minions/Melee/MeleeMinion.h"

AMonsterManager::AMonsterManager():
	m_MinTime(1.0f),
	m_MaxTime(5.0f)
{
	PrimaryActorTick.bCanEverTick = false; // Tick을 안쓰면 반드시 꺼놓기.

}

void AMonsterManager::BeginPlay()
{
	Super::BeginPlay();

	// 랜덤 생성시간 구하기
	float createTime = FMath::RandRange(m_MinTime, m_MaxTime);
	
	// 월드의 타임매니저에게 알림등록
	GetWorld()->GetTimerManager().SetTimer(m_SpawnTimerHandle, this, &AMonsterManager::CreateMonster, createTime);
}

void AMonsterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterManager::CreateMonster()
{
	// 랜덤위치 구하기
	int index = FMath::RandRange(0, m_SpawnPoints.Num() - 1);

	// 적 생성 및 배치
	//GetWorld()->SpawnActor<AMonster>(m_MonsterFactory, m_SpawnPoints[index]->GetActorLocation(), FRotator(0));
	GetWorld()->SpawnActor<AMeleeMinion>(m_MonsterFactory, m_SpawnPoints[index]->GetActorLocation(), FRotator(0));

	// 다시 랜덤 시간에 CreateEnemy 함수가 호출되도록 타이머 설정
	float createTime = FMath::FRandRange(m_MinTime, m_MaxTime);
	GetWorld()->GetTimerManager().SetTimer(m_SpawnTimerHandle, this, &AMonsterManager::CreateMonster, createTime);

}

