// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatComponent.h"

UStatComponent::UStatComponent() :
	m_MaxHP(200.0f),
	m_CurHP(m_MaxHP),
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

void UStatComponent::SetDamage(const float damage)
{
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

void UStatComponent::AddMoveSpeed(const float additionalMoveSpeed)
{
	m_CurAdditionalMoveSpeed += additionalMoveSpeed;
	m_CurMoveSpeed = 1.0f + m_CurAdditionalMoveSpeed / 100.0f;
}

void UStatComponent::AddAttackSpeed(const float additionalAttackSpeed)
{
	m_CurAdditionalAttackSpeed += additionalAttackSpeed;
	m_CurAttackSpeed = 1.0f + m_CurAdditionalAttackSpeed / 100.0f;
}



