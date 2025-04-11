// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/NotifyState/NotifyState_Check_KeyInput.h"
#include "Component/MainPlayerSkillComponent.h"
#include "MainPlayer/MainPlayer.h"

UNotifyState_Check_KeyInput::UNotifyState_Check_KeyInput()
{
}


void UNotifyState_Check_KeyInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AMainPlayer* owner = Cast<AMainPlayer>(MeshComp->GetOwner());
	if (owner != nullptr)
	{
		UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(owner->GetSkillComponent());
		if (ownerSkillComponent != nullptr)
		{
			ownerSkillComponent->SetHasStartedComboKeyInputCheck(true);
		}
	}
}

void UNotifyState_Check_KeyInput::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AMainPlayer* owner = Cast<AMainPlayer>(MeshComp->GetOwner());
	if (owner != nullptr)
	{
		UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(owner->GetSkillComponent());
		if (ownerSkillComponent != nullptr)
		{
			ownerSkillComponent->SetHasStartedComboKeyInputCheck(false);
		}
	}
}
