// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"
#include "SubSystems/UIManager.h"
#include "MainPlayer/Skill/Skill.h"


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
}


void UMainPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_Owner = Cast<AMainPlayer>(GetOwner());
	m_OwnerAnimInstance = Cast<UMainPlayerAnim>(m_Owner->GetMesh()->GetAnimInstance());
	
	initSkills();
	bindFuncOnMontageEvent();

	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateSkillSlots(this, m_Owner.Get());
}

void UMainPlayerSkillComponent::NormalAttack_OnGround()
{
	ExecuteSkill("DefaultOnGround","NormalAttack_OnGround");
}

void UMainPlayerSkillComponent::NormalAttack_InAir()
{
	ExecuteSkill("InAir","NormalAttack_InAir");
}

void UMainPlayerSkillComponent::UpperAttack_OnGround()
{
	ExecuteSkill("DefaultOnGround","UpperAttack_OnGround");
}

void UMainPlayerSkillComponent::DashAttack_OnGround()
{
	ExecuteSkill("DefaultOnGround","DashAttack_OnGround");
}

void UMainPlayerSkillComponent::DashAttack_InAir()
{
	ExecuteSkill("InAir","DashAttack_InAir");
}

void UMainPlayerSkillComponent::Dodge_OnGround()
{
	ExecuteSkill("DefaultOnGround","Dodge_OnGround");
}

void UMainPlayerSkillComponent::EarthStrike_InAir()
{
	ExecuteSkill("InAir","EarthStrike_InAir");
}

void UMainPlayerSkillComponent::Charging_OnGround()
{
	ExecuteSkill("ChargingOnGround","Charging_OnGround");
}

void UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround()
{
	ExecuteSkill("ChargingOnGround","ComboDashAttack_OnGround");
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
	m_OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &UMainPlayerSkillComponent::SetIdle); // 어떤 스킬이든 '끝까지(not Interrupted)' 재생 후 Idle로 전환.
}

bool UMainPlayerSkillComponent::GetIsChargingOnGround() const
{
	return m_CurSkillState == EMainPlayerSkillStates::Charging_OnGround;
}

void UMainPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, // 로그출력용 틱
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString curSkillState = Utility::ConvertEnumToString(m_CurSkillState);
	
	//GEngine->AddOnScreenDebugMessage(50, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), normalAttack->GetCurComboAttackSection()));
	GEngine->AddOnScreenDebugMessage(51, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(52, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_bIsStrikeAttackActive));
	GEngine->AddOnScreenDebugMessage(53, 3.f, FColor::Green, FString::Printf(TEXT("==============================")));
}

FName UMainPlayerSkillComponent::GetHighestPriorityInputMappingContext()
{
	return m_Owner->GetHighestPriorityInputMappingContext();
}
