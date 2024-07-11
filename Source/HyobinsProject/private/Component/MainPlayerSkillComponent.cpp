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
	PrimaryComponentTick.bCanEverTick = true; // 로그출력용
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
		if (m_bHasStartedComboKeyInputCheck) // 섹션점프 구간이면,
		{
			m_bHasStartedComboKeyInputCheck = false;

			if (!m_bAfterEndColliderHitCheck) // 공격정보초기화
			{
				initNormalAttackInfo();
			}
			
			linqNextNormalAttackCombo(); // 섹션점프
		}
	}
}

void UMainPlayerSkillComponent::OnCalledNotify_NormalAttack_OnGround_Start_EachSection() // 각 공격섹션 시작 시 호출.
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
	
	m_Owner->RotateActorToKeyInputDirection(); // 공격시마다 키입력방향으로 회전.
}

void UMainPlayerSkillComponent::linqNextNormalAttackCombo()
{
	if (m_CurNormalAttackSection < 5) // 기본공격중인경우,
	{
		if (m_Owner->GetIsPressedKey("LeftShift") || m_HasleftShiftDecision) // 강공격키 눌려있으면,
		{
			m_CurNormalAttackSection += 4;
		}
		else if (m_CurNormalAttackSection < 4) // 기본공격마지막섹션 아닐경우,
		{
			m_CurNormalAttackSection += 1;
		}
		else
		{
			return;
		}
	}
	else // 강공격중이였다면
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

void UMainPlayerSkillComponent::OnCalled_NormalAttack_OnGround_Ended_EachSection() // 각 기본공격섹션 마지막에 호출.
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
	if (m_CurSkillState != EMainPlayerSkillStates::Idle) // 어떠한 공격이든 수행중이면
	{
		m_Owner->RotateActorToControllerYaw(); // 카메라 정면(컨트롤러의 Yaw로 회전)
		m_OwnerAnimInstance->StopAllMontages(0.1f); // 빠른 모션변환을 위해 현재 재생중인 몽타주 Stop.

		int32 combatDodgeSection = Utility::GetDirectionIndex(m_Owner->GetCurInputVertical(), m_Owner->GetCurInputHorizontal());
		m_OwnerAnimInstance->PlayMontage(CombatDodgeOnGroundName,1.2f);
		m_OwnerAnimInstance->JumpToMontageSection(CombatDodgeOnGroundName, combatDodgeSection);
	}
}

void UMainPlayerSkillComponent::OnCalled_CombatDodge_OnGround_Start_Move() // 전투회피이동 시작하는시점
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
	
	// 각 충돌체들
	m_OwnerAnimInstance->SwordCollider_End_HitCheck.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_SwordCollider_End_HitCheck);
	m_OwnerAnimInstance->ShieldForAttackCollider_End_HitCheck.AddUObject(this, &UMainPlayerSkillComponent::OnCalledNotify_ShieldForAttackCollider_End_HitCheck);
}

void UMainPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, // 로그출력용 틱
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString curSkillState = Utility::ConvertEnumToString(m_CurSkillState);
	GEngine->AddOnScreenDebugMessage(13, 3.f, FColor::Green, FString::Printf(TEXT("curAttackSection : %d"), m_CurNormalAttackSection));
	GEngine->AddOnScreenDebugMessage(14, 3.f, FColor::Green, FString::Printf(TEXT("MainPlayerSkillState : %s"), *curSkillState));
	GEngine->AddOnScreenDebugMessage(15, 3.f, FColor::Green, FString::Printf(TEXT("HasleftShiftDecision : %d"), m_HasleftShiftDecision));
}