// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Dodge_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
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

	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Dodge_OnGround"),
	[this]()
	{
		if (m_OwnerAnimInstance->IsCurrentMontage("Dodge_OnGround"))
		{
			m_Owner->SetIsSuperArmor(false);
		}
	});
}

void UDodge_OnGround::Execute()
{
	Super::Execute();

	if (!m_bIsCoolDownActive)
	{
		return;
	}
	
	m_bIsCoolDownActive = false;
	
	const EMainPlayerSkillStates curSkillState = m_OwnerSkillComponent->GetState();
	
	if (m_OwnerSkillComponent->GetCanDodge()) // 어떠한 공격이든 수행중이면
	{
		OnExecute.Broadcast(); // 지금은 UI업데이트용.

		m_Owner->SetCrowdControlState(ECrowdControlStates::None);
		m_Owner->ClearCrowdControlTimerHandle();
		m_Owner->SetIsSuperArmor(true);
		m_OwnerAnimInstance->StopAllMontages(0.0f);
		m_OwnerSkillComponent->SetCanDodge(false);
		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::Dodge_OnGround);
		

		// 키입력에 따른 컨트롤러방향벡터 구하기.
		const FVector controllerKeyInputDirection = m_Owner->GetControllerKeyInputDirectionVector(
			m_Owner->GetDirectionIndexFromKeyInput());

		// 컨트롤러방향벡터를 캐릭터 로컬벡터로 변환해 캐릭터기준 방향 알아오기.
		const int32 localDirection = m_Owner->GetLocalDirection(controllerKeyInputDirection);

		m_OwnerAnimInstance->PlayMontage(TEXT("Dodge_OnGround"));
		m_OwnerAnimInstance->JumpToMontageSectionByIndex(TEXT("Dodge_OnGround"), localDirection);

		FVector targetVerticalVector;
		FVector targetHorizontalVector;

		if (m_Owner->GetCurInputHorizontal() == 0 && m_Owner->GetCurInputVertical() == 0) // 키입력 없을 시,
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
}
