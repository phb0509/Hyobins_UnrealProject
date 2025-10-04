// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/ChargingCancel_OnGround.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/EnumTypes.h"
#include "SubSystems/UIManager.h"

UChargingCancel_OnGround::UChargingCancel_OnGround() :
	m_StopChargingMontage(nullptr)
{
}

void UChargingCancel_OnGround::Initialize()
{
	Super::Initialize();

	check(m_StopChargingMontage != nullptr);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageNotInterruptedEnded(TEXT("StopCharging_OnGround"),
		[this]()
		{
			m_Owner->SetIsSuperArmor(false);
		});
}

void UChargingCancel_OnGround::Execute()
{
	Super::Execute();

	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
	check(ownerSkillComponent != nullptr);
	
	FTimerHandle chargingTimer = ownerSkillComponent->GetChargingTimer();
	
	m_Owner->GetWorldTimerManager().ClearTimer(chargingTimer);
	m_Owner->RemoveInputMappingContext(TEXT("Charging_OnGround"));
	m_Owner->AddInputMappingContext(TEXT("Default_OnGround"));

	ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::StopCharging);
	ownerSkillComponent->SetCanChargingSkill(false);

	m_OwnerAnimInstance->Montage_Play(m_StopChargingMontage,1.0f);

	UUIManager* uiManager = m_Owner->GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(uiManager != nullptr);
	
	uiManager->RemoveWidgetContainers("ChargingGageBar");
}

bool UChargingCancel_OnGround::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() && m_OwnerAnimInstance->IsCurrentMontage(TEXT("Charging_OnGround"));
}
