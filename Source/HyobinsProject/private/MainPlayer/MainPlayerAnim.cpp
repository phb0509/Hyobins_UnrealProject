// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"


UMainPlayerAnim::UMainPlayerAnim() 
{
}

void UMainPlayerAnim::ExecEvent_OnEnteredState_Falling() const
{
	AMainPlayer* owner = Cast<AMainPlayer>(TryGetPawnOwner());
	
	owner->AddInputMappingContext(TEXT("InAir"));
}

void UMainPlayerAnim::ExecEvent_OnEnteredState_MoveOnGround() const
{
	AMainPlayer* owner = Cast<AMainPlayer>(TryGetPawnOwner());
	
	owner->RemoveInputMappingContext(TEXT("InAir"));
}



