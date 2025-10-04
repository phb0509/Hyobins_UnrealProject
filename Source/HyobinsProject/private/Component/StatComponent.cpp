// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatComponent.h"
#include "CharacterBase/CharacterBase.h"

UStatComponent::UStatComponent() :
	m_DefaultDamage(100.0f),
	m_CriticalAttackChance(30.0f),
	m_Defense(0.0f),
	m_AdditionalDefenseFromGuard(0.0f),
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
	m_CurAdditionalAttackSpeed(0.0f)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Owner = Cast<ACharacterBase>(GetOwner());
	check(m_Owner != nullptr);
	
	InitHP();
}

void UStatComponent::OnDamageHP(const float damage)
{
	m_CurHP = FMath::Clamp<float>(m_CurHP - damage, 0.0f, m_MaxHP); // 상수값
	
	if (m_CurHP < KINDA_SMALL_NUMBER) 
	{
		m_CurHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
	else
	{
		RecoveryHP();
	}

	OnDamagedHP.Broadcast(); // UI체력바 업데이트.
}

void UStatComponent::RecoveryHP()
{
	if (m_Owner->IsGroggy())
	{
		return;
	}
	
	if (!m_Owner->GetWorld()->GetTimerManager().IsTimerActive(m_HPRecoveryTimer))
	{
		m_Owner->GetWorld()->GetTimerManager().SetTimer(m_HPRecoveryTimer,
		[this]()
		{
			m_CurHP += m_HPRecoveryPerSecond / 100;
			m_CurHP = m_CurHP >= m_MaxHP ? m_MaxHP : m_CurHP;
			
			if (m_CurHP >= m_MaxHP)
			{
				m_Owner->GetWorld()->GetTimerManager().ClearTimer(m_HPRecoveryTimer);
			}
			
			OnRecoveredHP.Broadcast();
		},
	0.01f,
	true);
	}
}

void UStatComponent::StopRecoveryHP()
{
	m_Owner->GetWorld()->GetTimerManager().ClearTimer(m_HPRecoveryTimer);
}

bool UStatComponent::CanRecoveryHP() const
{
	return m_Owner->GetWorld()->GetTimerManager().IsTimerActive(m_HPRecoveryTimer);
}

void UStatComponent::SetHPPercent(const float hp)
{
	m_CurHP = m_MaxHP * hp / 100.0f;
	
	OnDamagedHP.Broadcast();
}

void UStatComponent::OnDamageStamina(const float cost)
{
	m_CurStamina = FMath::Clamp<float>(m_CurStamina - cost, 0.0f, m_MaxStamina);
	
	this->OnDamagedStamina.Broadcast();
	
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

void UStatComponent::RecoveryStamina()
{
	if (!m_Owner->GetWorld()->GetTimerManager().IsTimerActive(m_StaminaRecoveryTimer))
	{
		m_Owner->GetWorld()->GetTimerManager().SetTimer(m_StaminaRecoveryTimer,
		[this]()
		{
			m_CurStamina += m_StaminaRecoveryPerSecond / 100;
			m_CurStamina = m_CurStamina >= m_MaxStamina ? m_MaxStamina : m_CurStamina;
    			
			if (m_CurStamina >= m_MaxStamina)
			{
				m_Owner->GetWorld()->GetTimerManager().ClearTimer(m_StaminaRecoveryTimer);
			}
    			
			OnRecoveredStamina.Broadcast();
		},
	0.01f,
	true);
	}
}

void UStatComponent::StopRecoveryStamina()
{
	m_Owner->GetWorld()->GetTimerManager().ClearTimer(m_StaminaRecoveryTimer);
}

bool UStatComponent::CanRecoveryStamina() const
{
	return m_Owner->GetWorld()->GetTimerManager().IsTimerActive(m_StaminaRecoveryTimer);
}

float UStatComponent::GetGroggyTime() const
{
	return m_Owner->GetGroggyTime();
}

void UStatComponent::SetStaminaPercent(const float stamina)
{
	m_CurStamina = m_MaxStamina * stamina / 100.0f;
	
	OnDamagedStamina.Broadcast();
}





