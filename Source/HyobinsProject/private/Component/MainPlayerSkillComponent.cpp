// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"
#include "MatineeCameraShake.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystems/UIManager.h"

#include "MainPlayer/Skill/NormalAttack_OnGround.h"
#include "MainPlayer/Skill/NormalAttack_InAir.h"
#include "MainPlayer/Skill/UpperAttack_OnGround.h"
#include "MainPlayer/Skill/DashAttack_InAir.h"
#include "MainPlayer/Skill/DashAttack_OnGround.h"
#include "MainPlayer/Skill/Dodge_OnGround.h"
#include "MainPlayer/Skill/Charging_OnGround.h"
#include "MainPlayer/Skill/ComboDashAttack_OnGround.h"
#include "MainPlayer/Skill/EarthStrike_InAir.h"


UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_CurSkillState(EMainPlayerSkillStates::Idle),
	m_GravityScaleInAir(0.00001f),
	m_bCanDodge(true),
	m_bCanChargingSkill(false),
	m_bHasStartedComboKeyInputCheck(false),
	m_bIsStrikeAttackActive(false),
	m_StrikeAttackDecisionTime(0.5f)
{
	PrimaryComponentTick.bCanEverTick = true; // 로그출력용

	//loadSkills();
}


void UMainPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_Owner = Cast<AMainPlayer>(GetOwner());
	m_OwnerAnimInstance = Cast<UMainPlayerAnim>(m_Owner->GetMesh()->GetAnimInstance());

	loadSkills();
	initSkills();
	bindFuncOnMontageEvent();

	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateSkillSlots(this);
}

void UMainPlayerSkillComponent::NormalAttack_OnGround()
{
	if (canNonChargingSkill_OnGround())
	{
		m_SkillList["DefaultOnGround"].skillList["NormalAttack_OnGround"]->Execute();
	}
}

void UMainPlayerSkillComponent::NormalAttack_InAir()
{
	m_SkillList["InAir"].skillList["NormalAttack_InAir"]->Execute();
}

void UMainPlayerSkillComponent::UpperAttack_OnGround()
{
	if (canNonChargingSkill_OnGround())
	{
		m_SkillList["DefaultOnGround"].skillList["UpperAttack_OnGround"]->Execute();
	}
}

void UMainPlayerSkillComponent::DashAttack_OnGround()
{
	if (canNonChargingSkill_OnGround())
	{
		m_SkillList["DefaultOnGround"].skillList["DashAttack_OnGround"]->Execute();
	}
}

void UMainPlayerSkillComponent::DashAttack_InAir()
{
	m_SkillList["InAir"].skillList["DashAttack_InAir"]->Execute();
}

void UMainPlayerSkillComponent::Dodge_OnGround()
{
	if (canNonChargingSkill_OnGround())
	{
		m_SkillList["DefaultOnGround"].skillList["Dodge_OnGround"]->Execute();
	}
}

void UMainPlayerSkillComponent::EarthStrike_InAir()
{
	m_SkillList["InAir"].skillList["EarthStrike_InAir"]->Execute();
}

void UMainPlayerSkillComponent::Charging_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		m_SkillList["ChargingOnGround"].skillList["Charging_OnGround"]->Execute();
	}
}

void UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround()
{
	if (m_Owner->GetIsOnGround())
	{
		m_SkillList["ChargingOnGround"].skillList["ComboDashAttack_OnGround"]->Execute();
	}
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

void UMainPlayerSkillComponent::InitGravityScaleAfterAttack()
{
	if (m_Owner->GetCharacterMovement()->GravityScale == m_GravityScaleInAir)
	{
		m_Owner->GetCharacterMovement()->GravityScale = 1.0f;
	}
}

void UMainPlayerSkillComponent::SetIdle(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		m_bCanDodge = true;
		m_CurSkillState = EMainPlayerSkillStates::Idle;
	}
}

void UMainPlayerSkillComponent::loadSkills()
{
	FSkillList skillList;
	m_SkillList.Add("DefaultOnGround", skillList);
	m_SkillList.Add("InAir", skillList);
	m_SkillList.Add("ChargingOnGround", skillList);

	
	// DefaultOnGround
	UNormalAttack_OnGround* normalAttack_OnGround = Utility::NewBlueprintObjectInConstructor<UNormalAttack_OnGround>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/NormalAttack_OnGround.NormalAttack_OnGround_C'",GetTransientPackage());
	m_SkillList["DefaultOnGround"].skillList.Add("NormalAttack_OnGround", normalAttack_OnGround);
	
	UUpperAttack_OnGround* upperAttack_OnGround = Utility::NewBlueprintObjectInConstructor<UUpperAttack_OnGround>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/UpperAttack_OnGround.UpperAttack_OnGround_C'",GetTransientPackage());
	m_SkillList["DefaultOnGround"].skillList.Add("UpperAttack_OnGround", upperAttack_OnGround);
	
	UDashAttack_OnGround* dashAttack_OnGround = Utility::NewBlueprintObjectInConstructor<UDashAttack_OnGround>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/DashAttack_OnGround.DashAttack_OnGround_C'",GetTransientPackage());
	m_SkillList["DefaultOnGround"].skillList.Add("DashAttack_OnGround", dashAttack_OnGround);

	UDodge_OnGround* dodge_OnGround = Utility::NewBlueprintObjectInConstructor<UDodge_OnGround>
("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/Dodge_OnGround.Dodge_OnGround_C'",GetTransientPackage());
	m_SkillList["DefaultOnGround"].skillList.Add("Dodge_OnGround", dodge_OnGround);

	
	// InAir
	UNormalAttack_InAir* normalAttack_InAir = Utility::NewBlueprintObjectInConstructor<UNormalAttack_InAir>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/NormalAttack_InAir.NormalAttack_InAir_C'",GetTransientPackage());
	m_SkillList["InAir"].skillList.Add("NormalAttack_InAir", normalAttack_InAir);
	
	UDashAttack_InAir* dashAttack_InAir = Utility::NewBlueprintObjectInConstructor<UDashAttack_InAir>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/DashAttack_InAir.DashAttack_InAir_C'",GetTransientPackage());
    m_SkillList["InAir"].skillList.Add("DashAttack_InAir", dashAttack_InAir);
	
	UEarthStrike_InAir* earthStrike_InAir = Utility::NewBlueprintObjectInConstructor<UEarthStrike_InAir>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/EarthStrike_InAir.EarthStrike_InAir_C'",GetTransientPackage());
	m_SkillList["InAir"].skillList.Add("EarthStrike_InAir", earthStrike_InAir);

	
	// ChargingOnGround
	UCharging_OnGround* charging_OnGround = Utility::NewBlueprintObjectInConstructor<UCharging_OnGround>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/Charging_OnGround.Charging_OnGround_C'",GetTransientPackage());
	m_SkillList["ChargingOnGround"].skillList.Add("Charging_OnGround", charging_OnGround);
	
	UComboDashAttack_OnGround* comboDashAttack_OnGround = Utility::NewBlueprintObjectInConstructor<UComboDashAttack_OnGround>
	("Blueprint'/Game/MainPlayerAsset/SkillBlueprint/ComboDashAttack_OnGround.ComboDashAttack_OnGround_C'",GetTransientPackage());
	m_SkillList["ChargingOnGround"].skillList.Add("ComboDashAttack_OnGround", comboDashAttack_OnGround);

}

void UMainPlayerSkillComponent::initSkills()
{
	for (const auto iter : m_SkillList)
	{
		for (const auto skill : iter.Value.skillList)
		{
			if (skill.Value != nullptr)
			{
				skill.Value->SetOwnerInfo(m_Owner.Get());
				skill.Value->Initialize();
			}
		}
	}
}

void UMainPlayerSkillComponent::bindFuncOnMontageEvent()
{
	// OnMontageEnded 
	m_OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &UMainPlayerSkillComponent::SetIdle); // 어떤 스킬이든 '끝까지(not Interrupted)' 재생 후 Idle로 전환.
}

bool UMainPlayerSkillComponent::canNonChargingSkill_OnGround() const
{
	return m_Owner->GetIsOnGround() && m_CurSkillState != EMainPlayerSkillStates::Charging_OnGround;
}

void UMainPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, // 로그출력용 틱
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString curSkillState = Utility::ConvertEnumToString(m_CurSkillState);
	//UNormalAttack_OnGround* normalAttack = Cast<UNormalAttack_OnGround>(m_SkillList["NormalAttack_OnGround"]);
	
	//GEngine->AddOnScreenDebugMessage(50, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), normalAttack->GetCurComboAttackSection()));
	GEngine->AddOnScreenDebugMessage(51, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(52, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_bIsStrikeAttackActive));
	GEngine->AddOnScreenDebugMessage(53, 3.f, FColor::Green, FString::Printf(TEXT("==============================")));
}

FName UMainPlayerSkillComponent::GetHighestPriorityInputMappingContext()
{
	return m_Owner->GetHighestPriorityInputMappingContext();
}
