// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"

const FName UMainPlayerSkillComponent::NormalAttackName = "NormalAttack";
const FName UMainPlayerSkillComponent::NormalStrikeAttackName = "NormalStrikeAttack";
const FName UMainPlayerSkillComponent::Dodge_NonTargeting = "Dodge_NonTargeting";
const FName UMainPlayerSkillComponent::Dodge_Targeting = "Dodge_Targeting";
const FName UMainPlayerSkillComponent::ParryName = "Parry";
const FName UMainPlayerSkillComponent::ParryingAttackName = "ParryingAttack";

UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_CurSkillState(EMainPlayerSkillStates::Idle),
	m_bHasStartedComboKeyInputCheck(false),
	m_CurNormalAttackSection(1),
	m_MaxNormalAttackSection(7),
	m_bHasleftShiftDecision(false),
	m_bSucceededParrying(false)
{
	PrimaryComponentTick.bCanEverTick = true; // �α���¿�
}

void UMainPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_Owner = Cast<AMainPlayer>(GetOwner());
	m_OwnerAnimInstance = Cast<UMainPlayerAnim>(m_Owner->GetMesh()->GetAnimInstance());
	bindFuncOnMontageEvent();
}

void UMainPlayerSkillComponent::NormalAttack()
{
	if (m_CurSkillState == EMainPlayerSkillStates::Idle) 
	{
		m_OwnerAnimInstance->PlayMontage(NormalAttackName, 1.1f);
	}
	else if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack ||    
			m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack)
	{
		if (m_bHasStartedComboKeyInputCheck) // �������� �����̸�,
		{
			m_bHasStartedComboKeyInputCheck = false;
			linqNextNormalAttackCombo(); // ��������
		}
	}
}

void UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_Start_EachSection() // �� ���ݼ��� ���� �� ȣ��.
{
	if (m_CurNormalAttackSection <= 4)
	{
		m_CurSkillState = EMainPlayerSkillStates::NormalAttack;
	}
	else
	{
		m_CurSkillState = EMainPlayerSkillStates::NormalStrikeAttack;
	}
	
	m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.
	m_Owner->GetCharacterMovement()->AddImpulse(m_Owner->GetActorForwardVector() * 3000.0f, true);
}

void UMainPlayerSkillComponent::linqNextNormalAttackCombo()
{
	if (m_CurNormalAttackSection % 2 != 0) // �⺻�������ΰ��,
	{
		if (m_Owner->GetIsPressedKey("LeftShift") || m_bHasleftShiftDecision) // ������Ű ����������,
		{
			m_CurNormalAttackSection += 1;
		}
		else  // ���� �⺻���� ����
		{
			m_CurNormalAttackSection += 2;
		}
	}
	else // ���������ΰ��,
	{
		if (m_Owner->GetIsPressedKey("LeftShift") || m_bHasleftShiftDecision)
		{
			m_CurNormalAttackSection += 2;
		}
		else
		{
			m_CurNormalAttackSection += 1;
		}
	}
	
	m_OwnerAnimInstance->JumpToMontageSection(NormalAttackName, m_CurNormalAttackSection);
}


void UMainPlayerSkillComponent::Dodge()
{
	if (m_CurSkillState != EMainPlayerSkillStates::Idle) // ��� �����̵� �������̸�
	{
		m_Owner->RotateActorToControllerYaw(); // ī�޶� ����(��Ʈ�ѷ��� Yaw�� ȸ��)
		m_OwnerAnimInstance->StopAllMontages(0.0f); // ���� ��Ǻ�ȯ�� ���� ���� ������� ��Ÿ�� Stop.

		int32 combatDodgeSection = Utility::GetDirectionIndex(m_Owner->GetCurInputVertical(), m_Owner->GetCurInputHorizontal());
		m_OwnerAnimInstance->PlayMontage(Dodge_NonTargeting,1.2f);
		m_OwnerAnimInstance->JumpToMontageSection(Dodge_NonTargeting, combatDodgeSection);
	}
}

void UMainPlayerSkillComponent::Parry()
{
	if (m_OwnerAnimInstance->Montage_IsPlaying(m_OwnerAnimInstance->GetMontage(ParryName)))
	{
		m_OwnerAnimInstance->PlayMontage(ParryingAttackName);
		m_Owner->GetCharacterMovement()->AddImpulse(m_Owner->GetActorForwardVector() * 10000.0f, true);
	}
	else
	{
		m_OwnerAnimInstance->PlayMontage(ParryName);
	}
	
	
	// if (m_bSucceededParrying)
	// {
	// 	ParryingAttack();
	// }
	// else
	// {
	// 	m_OwnerAnimInstance->PlayMontage(ParryName);
	// 	m_bSucceededParrying = true;
	// }
	
}

void UMainPlayerSkillComponent::ParryingAttack()
{
	m_OwnerAnimInstance->PlayMontage(ParryingAttackName);
	m_bSucceededParrying = false;
}


void UMainPlayerSkillComponent::ExtendShiftDecisionTime()
{
	m_bHasleftShiftDecision = true;
	
	m_Owner->GetWorldTimerManager().SetTimer
    	(
    		m_ShiftDecisionTimerHandle,
    		[this]()
    			{ m_bHasleftShiftDecision = false; },
    	0.1f,
    	false);
}

void UMainPlayerSkillComponent::bindFuncOnMontageEvent()
{
	// NormalAttack_OnGround
	m_OwnerAnimInstance->NormalAttack_Start_EachSection.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_Start_EachSection);
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageEnded(NormalAttackName,
	[this](){m_CurNormalAttackSection = 1;});
	
	// CombatDodge_OnGround
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageStarted(Dodge_NonTargeting,
		[this](){m_CurSkillState = EMainPlayerSkillStates::Dodge_NonTargeting;});
	
	// OnMontageEnded
	m_OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &UMainPlayerSkillComponent::SetIdle);

}

void UMainPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, // �α���¿� ƽ
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString curSkillState = Utility::ConvertEnumToString(m_CurSkillState);
	GEngine->AddOnScreenDebugMessage(13, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), m_CurNormalAttackSection));
	GEngine->AddOnScreenDebugMessage(14, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(15, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_bHasleftShiftDecision || m_Owner->GetIsPressedKey("LeftShift")));
}