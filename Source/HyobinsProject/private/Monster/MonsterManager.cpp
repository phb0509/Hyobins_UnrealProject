// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterManager.h"
#include "Monster/Monster.h"
#include "Monster/Minions/Melee/MeleeMinion.h"

AMonsterManager::AMonsterManager():
	m_MinTime(1.0f),
	m_MaxTime(5.0f)
{
	PrimaryActorTick.bCanEverTick = false; // Tick�� �Ⱦ��� �ݵ�� ������.

}

void AMonsterManager::BeginPlay()
{
	Super::BeginPlay();

	// ���� �����ð� ���ϱ�
	float createTime = FMath::RandRange(m_MinTime, m_MaxTime);
	
	// ������ Ÿ�ӸŴ������� �˸����
	GetWorld()->GetTimerManager().SetTimer(m_SpawnTimerHandle, this, &AMonsterManager::CreateMonster, createTime);
}

void AMonsterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterManager::CreateMonster()
{
	// ������ġ ���ϱ�
	int index = FMath::RandRange(0, m_SpawnPoints.Num() - 1);

	// �� ���� �� ��ġ
	//GetWorld()->SpawnActor<AMonster>(m_MonsterFactory, m_SpawnPoints[index]->GetActorLocation(), FRotator(0));
	GetWorld()->SpawnActor<AMeleeMinion>(m_MonsterFactory, m_SpawnPoints[index]->GetActorLocation(), FRotator(0));

	// �ٽ� ���� �ð��� CreateEnemy �Լ��� ȣ��ǵ��� Ÿ�̸� ����
	float createTime = FMath::FRandRange(m_MinTime, m_MaxTime);
	GetWorld()->GetTimerManager().SetTimer(m_SpawnTimerHandle, this, &AMonsterManager::CreateMonster, createTime);

}

