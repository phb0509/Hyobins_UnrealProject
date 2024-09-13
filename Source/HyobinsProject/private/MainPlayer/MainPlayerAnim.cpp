// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"


UMainPlayerAnim::UMainPlayerAnim() 
{
	
}

void UMainPlayerAnim::AnimNotify_NormalAttack_Start_EachSection() const
{
	NormalAttack_Start_EachSection.Broadcast();
}

void UMainPlayerAnim::AnimNotify_AirToGroundAttack_CollisionCheck() const
{
	AirToGroundAttack_CollisionCheck.Broadcast();
}


