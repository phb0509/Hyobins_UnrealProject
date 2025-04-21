// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Guard_OnGround.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Component/StatComponent.h"
#include "Utility/EnumTypes.h"


UGuard_OnGround::UGuard_OnGround() :
	m_AdditionalDefense(100.0f)
{
}

void UGuard_OnGround::Initialize()
{
	Super::Initialize();

	AMainPlayer* owner = Cast<AMainPlayer>(m_Owner);
	owner->GetStatComponent()->SetAdditionalDefenseFromGuard(m_AdditionalDefense);
	
	// m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Guard_OnGround"),
	// [=]()
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("UGuard_OnGround :: AllEnded"));
	// 	//owner->SetIsGuarding(false);
	// 	//owner->GetCollider(TEXT("ShieldForGuardCollider"))->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// });
}

void UGuard_OnGround::Execute()
{
	Super::Execute();

	AMainPlayer* owner = Cast<AMainPlayer>(m_Owner);

	if (!owner->IsGuard())
	{
		m_OwnerSkillComponent->SetSkillState(EMainPlayerSkillStates::Guard_OnGround);
		m_OwnerAnimInstance->PlayMontage(TEXT("Guard_OnGround"));

		owner->EnableGuard();
	}
	else // 정상적인 해제.
	{
		m_OwnerAnimInstance->PlayMontage(TEXT("GuardEnd_OnGround"));

		owner->DisableGuard();
	}
}

bool UGuard_OnGround::GetCanExecuteSkill() const
{
	return !m_Owner->IsCrowdControlState() &&
		!m_OwnerSkillComponent->IsCurSkillState(EMainPlayerSkillStates::Charging_OnGround);
}
