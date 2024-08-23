// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerController.h"


AMainPlayerController::AMainPlayerController():
	m_TeamID(FGenericTeamId(1))
{
	UE_LOG(LogTemp, Warning, TEXT("MainPlayerController::Constructor"));
	
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return m_TeamID;
}