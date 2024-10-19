// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatComponent.h"

UStatComponent::UStatComponent() :
	m_DefaultDamage(100.0f),
	m_CriticalAttackChance(30.0f),
	m_Defense(0.0f),
	m_MaxHP(200.0f),
	m_CurHP(m_MaxHP),
	m_HitRecovery(1.0f),
	m_CurMoveSpeed(1.0f),
	m_CurAdditionalMoveSpeed(0.0f),
	m_CurAttackSpeed(1.0f),
	m_CurAdditionalAttackSpeed(0.0f)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitHP();
}

void UStatComponent::OnDamage(const float damage)
{
	// 방어력이나 속성저항력등에 의해 실제로 깎아야하는 hp를 구한다.
	SetHP(FMath::Clamp<float>(m_CurHP - damage, 0.0f, m_MaxHP));
}

void UStatComponent::SetHP(const float hp)
{
	m_CurHP = hp;
	OnHPIsChanged.Broadcast();

	if (m_CurHP < KINDA_SMALL_NUMBER)
	{
		m_CurHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}





