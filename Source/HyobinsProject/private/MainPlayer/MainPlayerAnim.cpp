// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"


UMainPlayerAnim::UMainPlayerAnim() 
{
}

void UMainPlayerAnim::ExecEvent_OnEnteredState_Falling() const
{
	OnEnteredState_Falling.Broadcast();
}

void UMainPlayerAnim::ExecEvent_OnEnteredState_MoveOnGround() const
{
	OnEnteredState_MoveOnGround.Broadcast();
}



