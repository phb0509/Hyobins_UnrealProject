// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatComponent.h"
#include "Utility/CharacterBase.h"

UStatComponent::UStatComponent() :
	m_DefaultDamage(100.0f),
	m_CriticalAttackChance(30.0f),
	m_Defense(0.0f),
	m_MaxHP(200.0f),
	m_CurHP(m_MaxHP),
	m_MaxStamina(100.0f),
	m_CurStamina(m_MaxStamina),
	m_HitRecovery(1.0f),
	m_HPRecoveryPerSecond(500.0f),
	m_StaminaRecoveryPerSecond(2.0f),
	m_CurMoveSpeed(1.0f),
	m_CurAdditionalMoveSpeed(0.0f),
	m_CurAttackSpeed(1.0f),
	m_CurAdditionalAttackSpeed(0.0f),
	m_bCanRecoveryHP(true),
	m_bCanRecoveryStamina(true)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Owner = Cast<ACharacterBase>(GetOwner());
	InitHP();
}

void UStatComponent::RecoveryHP()
{
	if (!GetOwner()->GetWorld()->GetTimerManager().IsTimerActive(m_HPRecoveryTimer))
	{
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(m_HPRecoveryTimer,
		[this]()
		{
			if (!m_bCanRecoveryHP)
			{
				GetOwner()->GetWorld()->GetTimerManager().ClearTimer(m_HPRecoveryTimer);
				return;
			}
			
			m_CurHP += m_HPRecoveryPerSecond / 100;
			m_CurHP = m_CurHP >= m_MaxHP ? m_MaxHP : m_CurHP;
			
			if (m_CurHP >= m_MaxHP)
			{
				GetOwner()->GetWorld()->GetTimerManager().ClearTimer(m_HPRecoveryTimer);
			}
			
			OnChangedHP.Broadcast(0.0f);
		},
	0.01f,
	true);
	}
}

void UStatComponent::RecoveryStamina()
{
	if (!GetOwner()->GetWorld()->GetTimerManager().IsTimerActive(m_StaminaRecoveryTimer))
    	{
    		GetOwner()->GetWorld()->GetTimerManager().SetTimer(m_StaminaRecoveryTimer,
    		[this]()
    		{
    			if (!m_bCanRecoveryStamina)
    			{
					GetOwner()->GetWorld()->GetTimerManager().ClearTimer(m_StaminaRecoveryTimer);
					return;
				}
    			
    			m_CurStamina += m_StaminaRecoveryPerSecond / 100;
    			m_CurStamina = m_CurStamina >= m_MaxStamina ? m_MaxStamina : m_CurStamina;
    			
    			if (m_CurStamina >= m_MaxStamina)
    			{
    				GetOwner()->GetWorld()->GetTimerManager().ClearTimer(m_StaminaRecoveryTimer);
    			}
    			
    			OnChangedStamina.Broadcast(0.0f);
    		},
    	0.01f,
    	true);
    	}
}

void UStatComponent::OnDamageHP(const float damage)
{
	m_CurHP = FMath::Clamp<float>(m_CurHP - damage, 0.0f, m_MaxHP); // 상수값
	OnChangedHP.Broadcast(damage); // UI체력바 업데이트.
	
	if (m_CurHP < KINDA_SMALL_NUMBER) 
	{
		m_CurHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
	else
	{
		RecoveryHP();
	}
}

void UStatComponent::SetHPPercent(const float hp)
{
	m_CurHP = m_MaxHP * hp / 100.0f;
	OnChangedHP.Broadcast(0.0f);
}

void UStatComponent::OnDamageStamina(const float stamina)
{
	m_CurStamina = FMath::Clamp<float>(m_CurStamina - stamina, 0.0f, m_MaxStamina);
	OnChangedStamina.Broadcast(stamina);
	
	if (m_CurStamina < KINDA_SMALL_NUMBER)
	{
		m_CurStamina = 0.0f;
		OnStaminaIsZero.Broadcast();
	}
	else
	{
		RecoveryStamina();
	}
}

void UStatComponent::SetStaminaPercent(const float stamina)
{
	m_CurStamina = m_MaxStamina * stamina / 100.0f;
	OnChangedStamina.Broadcast(0.0f);
}





