// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MainPlayerSkillComponent.h"
#include "Components/ShapeComponent.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Utility/Utility.h"
#include "Utility/EnumTypes.h"

const FName UMainPlayerSkillComponent::NormalAttackOnGroundName = "NormalAttack_OnGround";
const FName UMainPlayerSkillComponent::NormalStrikeAttackOnGroundName = "NormalStrikeAttack_OnGround";
const FName UMainPlayerSkillComponent::CombatDodgeOnGroundName = "CombatDodge_OnGround";

UMainPlayerSkillComponent::UMainPlayerSkillComponent() :
	m_CurSkillState(EMainPlayerSkillStates::Idle),
	m_bAfterEndColliderHitCheck(false),
	m_bHasStartedComboKeyInputCheck(false),
	m_CurNormalAttackSection(1),
	m_MaxNormalAttackSection(8),
	m_HasleftShiftDecision(false)
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

void UMainPlayerSkillComponent::NormalAttack_OnGround()
{
	if (m_CurSkillState == EMainPlayerSkillStates::Idle) 
	{
		m_OwnerAnimInstance->PlayMontage(NormalAttackOnGroundName, 1.1f);
	}
	else if (m_CurSkillState == EMainPlayerSkillStates::NormalAttack_OnGround ||    
			m_CurSkillState == EMainPlayerSkillStates::NormalStrikeAttack_OnGround)
	{
		if (m_bHasStartedComboKeyInputCheck) // �������� �����̸�,
		{
			m_bHasStartedComboKeyInputCheck = false;

			if (!m_bAfterEndColliderHitCheck) // ���������ʱ�ȭ
			{
				initNormalAttackInfo();
			}
			
			linqNextNormalAttackCombo(); // ��������
		}
	}
}

void UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_OnGround_Start_EachSection() // �� ���ݼ��� ���� �� ȣ��.
{
	m_bAfterEndColliderHitCheck = false;
	
	if (m_CurNormalAttackSection < 5)
	{
		m_CurSkillState = EMainPlayerSkillStates::NormalAttack_OnGround;
	}
	else
	{
		m_CurSkillState = EMainPlayerSkillStates::NormalStrikeAttack_OnGround;
	}
	
	m_Owner->RotateActorToKeyInputDirection(); // ���ݽø��� Ű�Է¹������� ȸ��.
}

void UMainPlayerSkillComponent::linqNextNormalAttackCombo()
{
	if (m_CurNormalAttackSection < 5) // �⺻�������ΰ��,
	{
		if (m_Owner->GetIsPressedKey("LeftShift") || m_HasleftShiftDecision) // ������Ű ����������,
		{
			m_CurNormalAttackSection += 4;
		}
		else if (m_CurNormalAttackSection < 4) // �⺻���ݸ��������� �ƴҰ��,
		{
			m_CurNormalAttackSection += 1;
		}
		else
		{
			return;
		}
	}
	else // ���������̿��ٸ�
	{
		if (m_Owner->GetIsPressedKey("LeftShift") || m_HasleftShiftDecision)
		{
			m_CurNormalAttackSection += 1;
		}
		else
		{
			m_CurNormalAttackSection -= 3;
		}
	}

	m_OwnerAnimInstance->JumpToMontageSection(NormalAttackOnGroundName, m_CurNormalAttackSection);
}

void UMainPlayerSkillComponent::OnCalled_NormalAttack_OnGround_Ended_EachSection() // �� �⺻���ݼ��� �������� ȣ��.
{
	if (!m_bAfterEndColliderHitCheck)
	{
		initNormalAttackInfo();
	}
	
	m_CurNormalAttackSection = 1;
	m_bAfterEndColliderHitCheck = false;
}

void UMainPlayerSkillComponent::initNormalAttackInfo()
{
	m_CurNormalAttackSection < 5
	? m_Owner->EmptyCheckedHitActor(NormalAttackOnGroundName)
	: m_Owner->EmptyCheckedHitActor(NormalStrikeAttackOnGroundName);

	m_CurNormalAttackSection != 3
	? m_Owner->GetCollider(AMainPlayer::SwordColliderName)->SetCollisionEnabled(ECollisionEnabled::NoCollision)
	: m_Owner->GetCollider(AMainPlayer::ShieldForAttackColliderName)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UMainPlayerSkillComponent::Dodge_OnGround()
{
	if (m_CurSkillState != EMainPlayerSkillStates::Idle) // ��� �����̵� �������̸�
	{
		m_Owner->RotateActorToControllerYaw(); // ī�޶� ����(��Ʈ�ѷ��� Yaw�� ȸ��)
		m_OwnerAnimInstance->StopAllMontages(0.1f); // ���� ��Ǻ�ȯ�� ���� ���� ������� ��Ÿ�� Stop.

		int32 combatDodgeSection = Utility::GetDirectionIndex(m_Owner->GetCurInputVertical(), m_Owner->GetCurInputHorizontal());
		m_OwnerAnimInstance->PlayMontage(CombatDodgeOnGroundName,1.2f);
		m_OwnerAnimInstance->JumpToMontageSection(CombatDodgeOnGroundName, combatDodgeSection);
	}
}

void UMainPlayerSkillComponent::OnCalled_CombatDodge_OnGround_Start_Move() // ����ȸ���̵� �����ϴ½���
{
	m_CurSkillState = EMainPlayerSkillStates::CombatDodge_OnGround;
}

void UMainPlayerSkillComponent::UpdateShiftDecisionTime()
{
	m_HasleftShiftDecision = true;

	m_Owner->GetWorldTimerManager().SetTimer
	(
		m_ShiftDecisionTimerHandle,
		FTimerDelegate::CreateLambda([this]()
			{ m_HasleftShiftDecision = false; }),
	0.3f,
	false);
}

void UMainPlayerSkillComponent::bindFuncOnMontageEvent()
{
	// NormalAttack_OnGround
	m_OwnerAnimInstance->NormalAttack_Start_EachSection.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_OnGround_Start_EachSection);
	m_OwnerAnimInstance->BindFuncOnMontageEnded(NormalAttackOnGroundName, this, TEXT("OnCalled_NormalAttack_OnGround_Ended_EachSection"));
	
	// CombatDodge_OnGround
	m_OwnerAnimInstance->BindFuncOnMontageStarted(CombatDodgeOnGroundName, this, TEXT("OnCalled_CombatDodge_OnGround_Start_Move"));
	
	// OnMontageEnded
	m_OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &UMainPlayerSkillComponent::SetIdle);
	
	// �� �浹ü��
	m_OwnerAnimInstance->SwordCollider_End_HitCheck.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_SwordCollider_End_HitCheck);
	m_OwnerAnimInstance->ShieldForAttackCollider_End_HitCheck.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_ShieldForAttackCollider_End_HitCheck);
}

void UMainPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, // �α���¿� ƽ
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString curSkillState = Utility::ConvertEnumToString(m_CurSkillState);
	GEngine->AddOnScreenDebugMessage(13, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), m_CurNormalAttackSection));
	GEngine->AddOnScreenDebugMessage(14, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(15, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_HasleftShiftDecision));
}