// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatComponent.h"

UStatComponent::UStatComponent() :
	m_MaxHP(200.0f),
	m_CurHP(m_MaxHP)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitHP();
}

FORCEINLINE float UStatComponent::GetHPRatio() const
{
	return m_CurHP < KINDA_SMALL_NUMBER ? 0.0f : (m_CurHP / m_MaxHP);
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

