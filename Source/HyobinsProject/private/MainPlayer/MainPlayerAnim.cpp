// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"


UMainPlayerAnim::UMainPlayerAnim() 
{
	
}

void UMainPlayerAnim::AnimNotify_NormalAttack_Start_EachSection() const
{
	NormalAttack_Start_EachSection.Broadcast();
}

void UMainPlayerAnim::ExecEvent_OnEnteredState_Falling() const
{
	OnEnteredState_Falling.Broadcast();
}

void UMainPlayerAnim::ExecEvent_OnEnteredState_MoveOnGround() const
{
	OnEnteredState_MoveOnGround.Broadcast();
}



