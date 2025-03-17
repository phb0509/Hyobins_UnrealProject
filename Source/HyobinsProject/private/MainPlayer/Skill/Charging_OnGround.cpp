// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Charging_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/EnumTypes.h"
#include "SubSystems/UIManager.h"

UCharging_OnGround::UCharging_OnGround() :
	m_ChargingDuration(2.0f)
{
}

void UCharging_OnGround::Initialize()
{
	Super::Initialize();
	
	UUIManager* uiManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	OnCharging.BindUObject(uiManager, &UUIManager::CreateChargingGageBar);
	OnStopCharging.BindUObject(uiManager, &UUIManager::RemoveChargingGageBar);
}

void UCharging_OnGround::Execute()
{
	Super::Execute();

	if (!m_bIsCoolDownActive)
	{
		return;
	}
	
	m_bIsCoolDownActive = false;
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetSkillState();

	OnExecute.Broadcast(); // UI
	
	if (curSkillState == EMainPlayerSkillStates::Charging_OnGround)
	{
		m_Owner->GetWorldTimerManager().ClearTimer(m_ChargingTimer);
		m_Owner->RemoveInputContextMappingOnCharging();
		
		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::StopCharging);
		m_OwnerSkillComponent->SetCanChargingSkill(false);
		
		m_OwnerAnimInstance->PlayMontage(TEXT("StopCharging_OnGround"));
		
		const bool result = OnStopCharging.ExecuteIfBound(m_Owner.Get()); // UI
	}
	else
	{
		m_Owner->GetWorldTimerManager().SetTimer(m_ChargingTimer,
					[this]()
					{
						m_Owner->AddInputContextMappingOnCharging();
						m_OwnerSkillComponent->SetCanChargingSkill(true);
					},
				m_ChargingDuration,
				false);

		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::Charging_OnGround);
		
		m_OwnerAnimInstance->PlayMontage(TEXT("Charging_OnGround"));
		
		OnCharging.ExecuteIfBound(m_Owner.Get(), m_ChargingDuration); // UI
	}
}
