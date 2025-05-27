// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Charging_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
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
}

void UCharging_OnGround::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	FTimerHandle chargingTimer = ownerSkillComponent->GetChargingTimer();
	
	m_Owner->GetWorldTimerManager().SetTimer(
		chargingTimer,
		[=]()
		{
			m_Owner->RemoveInputMappingContext(TEXT("Default_OnGround"));
			m_Owner->AddInputMappingContext(TEXT("Charging_OnGround"));
			ownerSkillComponent->SetCanChargingSkill(true);
		},
		m_ChargingDuration,
		false);

	ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::Charging_OnGround);
	
	m_OwnerAnimInstance->PlayMontage(TEXT("Charging_OnGround"));
	
	UUIManager* uiManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	uiManager->CreateChargingGageBar(m_Owner.Get(), m_ChargingDuration);
}

bool UCharging_OnGround::CanExecuteSkill() const
{
	return !m_Owner->IsCrowdControlState();
}
