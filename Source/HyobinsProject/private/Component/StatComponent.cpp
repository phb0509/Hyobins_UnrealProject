// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatComponent.h"

UStatComponent::UStatComponent() :
	m_DefaultDamage(100.0f),
	m_CriticalAttackChance(30.0f),
	m_Defense(0.0f),
	m_MaxHP(200.0f),
	m_CurHP(m_MaxHP),
	m_MaxStamina(100.0f),
	m_CurStamina(m_MaxStamina),
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

void UStatComponent::UpdateHP(const float damage)
{
	SetHP(FMath::Clamp<float>(m_CurHP - damage, 0.0f, m_MaxHP));
}

void UStatComponent::SetHP(const float hp)
{
	m_CurHP = hp;
	OnChangedHP.Broadcast();

	if (m_CurHP < KINDA_SMALL_NUMBER)
	{
		m_CurHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UStatComponent::UpdateStamina(const float stamina)
{
	SetStamina(FMath::Clamp<float>(m_CurStamina - stamina, 0.0f, m_MaxStamina));

	UE_LOG(LogTemp, Warning, TEXT("UStatComponent :: UpdateStamina"));
}

void UStatComponent::SetStamina(const float stamina)
{
	m_CurStamina = stamina;
	OnChangedStamina.Broadcast();

	if (m_CurStamina < KINDA_SMALL_NUMBER)
	{
		m_CurStamina = 0.0f;
		OnStaminaIsZero.Broadcast();
	}
}





