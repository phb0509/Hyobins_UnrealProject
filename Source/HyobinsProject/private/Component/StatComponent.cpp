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

void UStatComponent::SetDamage(float damage)
{
	m_CurHP -= damage;

	if (m_CurHP <= 0.0f)
	{
		m_CurHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}



