// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"
#include "SubSystems/DebugManager.h"


UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_bCanChargingSkill(false),
	m_bHasStartedComboKeyInputCheck(false),
	m_bIsStrikeAttackActive(false),
	m_StrikeAttackDecisionTime(0.5f)
{
	PrimaryComponentTick.bCanEverTick = false; 
}

void UMainPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
	bindFuncOnMontageEvent();
	
	UDebugManager* debugManager = GetOwner()->GetGameInstance()->GetSubsystem<UDebugManager>();
	check(debugManager != nullptr);

	debugManager->OnDebugMode.AddUObject(this, &UMainPlayerSkillComponent::printLog);
}

// Default_OnGround
void UMainPlayerSkillComponent::NormalAttack_OnGround()
{
	ExecuteSkill("Default_OnGround","NormalAttack_OnGround");
}

void UMainPlayerSkillComponent::UpperAttack_OnGround()
{
	ExecuteSkill("Default_OnGround","UpperAttack_OnGround");
}

void UMainPlayerSkillComponent::DashAttack_OnGround()
{
	ExecuteSkill("Default_OnGround","DashAttack_OnGround");
}

void UMainPlayerSkillComponent::Dodge_OnGround()
{
	ExecuteSkill("Default_OnGround","Dodge_OnGround");
}

void UMainPlayerSkillComponent::Guard_OnGround()
{
	ExecuteSkill("Default_OnGround","Guard_OnGround");
}

void UMainPlayerSkillComponent::Charging_OnGround()
{
	ExecuteSkill("Default_OnGround","Charging_OnGround");
}

void UMainPlayerSkillComponent::Execution_OnGround()
{
	ExecuteSkill("Default_OnGround","Execution_OnGround");
}


// Default_InAir
void UMainPlayerSkillComponent::NormalAttack_InAir()
{
	ExecuteSkill("Default_InAir","NormalAttack_InAir");
}

void UMainPlayerSkillComponent::EarthStrike_InAir()
{
	ExecuteSkill("Default_InAir","EarthStrike_InAir");
}

void UMainPlayerSkillComponent::DashAttack_InAir()
{
	ExecuteSkill("Default_InAir","DashAttack_InAir");
}


// Charging_OnGround
void UMainPlayerSkillComponent::ChargingCancel_OnGround()
{
	ExecuteSkill("Charging_OnGround","ChargingCancel_OnGround");
}

void UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround()
{
	ExecuteSkill("Charging_OnGround","ComboDashAttack_OnGround");
}

void UMainPlayerSkillComponent::FateSealed()
{
	ExecuteSkill("Charging_OnGround","FateSealed");
}


void UMainPlayerSkillComponent::ExtendStrikeActivateDecisionTime()
{
	FTimerHandle timer;
	m_Owner->GetWorldTimerManager().SetTimer(timer,
	[this]()
	{
		m_bIsStrikeAttackActive = false;
	},
	m_StrikeAttackDecisionTime,
	false);
}


void UMainPlayerSkillComponent::SetIdle(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		SetSkillState(EMainPlayerSkillStates::None);
	}
}

void UMainPlayerSkillComponent::bindFuncOnMontageEvent()
{
	m_OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &UMainPlayerSkillComponent::SetIdle); // 어떤 스킬이든 '끝까지(not Interrupted)' 재생 후 Idle로 전환.
}

void UMainPlayerSkillComponent::printLog()
{
	const FString curSkillState = Utility::ConvertEnumToString(static_cast<EMainPlayerSkillStates>(m_CurSkillState));
	const FName curMontageName = m_OwnerAnimInstance->GetCurrentMontageName();
	
	//GEngine->AddOnScreenDebugMessage(50, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), normalAttack->GetCurComboAttackSection()));
	GEngine->AddOnScreenDebugMessage(200, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(201, 3.f, FColor::Green, FString::Printf(TEXT("Current Montage : %s"), *curMontageName.ToString()));
	GEngine->AddOnScreenDebugMessage(202, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_bIsStrikeAttackActive));
	GEngine->AddOnScreenDebugMessage(203, 3.f, FColor::Green, FString::Printf(TEXT("==============================")));
}




