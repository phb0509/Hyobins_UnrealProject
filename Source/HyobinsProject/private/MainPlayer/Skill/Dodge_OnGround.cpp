// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Dodge_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MotionWarpingComponent.h"
#include "Utility/EnumTypes.h"

UDodge_OnGround::UDodge_OnGround() :
	m_MoveDistance(400.0f)
{
}

void UDodge_OnGround::Initialize()
{
	Super::Initialize();
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Dodge_NonTargeting_OnGround"),
	[this]()
	{
		if (m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Idle))
		{
			m_Owner->SetIsSuperArmor(false, false);
		}
	});
}

void UDodge_OnGround::Execute()
{
	Super::Execute();
	
	UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);

	m_Owner->SetCrowdControlState(ECrowdControlStates::None); // CC상태에서도 시전가능.
	m_Owner->ClearCrowdControlTimerHandle();
	
	m_OwnerAnimInstance->StopAllMontages(0.0f);
	
	ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::Dodge_NonTargeting_OnGround);

	// 키입력에 따른 월드방향벡터 구하기.
	const FVector worldKeyInputDirection = m_Owner->GetWorldKeyInputDirection(m_Owner->GetDirectionIndexFromKeyInput());

	// 캐릭터기준으로 어디방향인지 구하기.
	const int32 localDirectionIndex = m_Owner->GetLocalDirectionIndex(worldKeyInputDirection);
	
	m_OwnerAnimInstance->PlayMontage(TEXT("Dodge_NonTargeting_OnGround"));
	m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("Dodge_NonTargeting_OnGround"), localDirectionIndex);

	FVector targetVerticalVector;
	FVector targetHorizontalVector;
	
	if (!m_Owner->IsMoveKeyPressed()) // 키입력 없을 시,
	{
		targetVerticalVector = m_Owner->GetForwardVectorFromControllerYaw() * m_MoveDistance;
		targetHorizontalVector = {0.0f, 0.0f, 0.0f};
	}
	else
	{
		targetVerticalVector = m_Owner->GetForwardVectorFromControllerYaw() * m_MoveDistance * m_Owner
			->GetCurInputVertical();
		targetHorizontalVector = m_Owner->GetRightVectorFromControllerYaw() * m_MoveDistance * m_Owner
			->GetCurInputHorizontal();
	}

	m_Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("Forward"), m_Owner->GetActorLocation() + targetVerticalVector + targetHorizontalVector);
	
}

bool UDodge_OnGround::CanExecuteSkill() const
{
	return !m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround);
}
