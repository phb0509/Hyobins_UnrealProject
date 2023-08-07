// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatComponent.h"

UStatComponent::UStatComponent() :
	m_MaxHP(100.0f),
	m_CurHP(m_MaxHP)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	InitHP();
}

void UStatComponent::SetHP(float hp)
{
	m_CurHP = hp;
	OnHPIsChanged.Broadcast();

	if (m_CurHP < KINDA_SMALL_NUMBER)
	{
		m_CurHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UStatComponent::SetDamage(float damage)
{
	SetHP(FMath::Clamp<float>(m_CurHP - damage, 0.0f, m_MaxHP));
}

float UStatComponent::GetHPRatio()
{
	return m_CurHP < KINDA_SMALL_NUMBER ? 0.0f : (m_CurHP / m_MaxHP);
}



