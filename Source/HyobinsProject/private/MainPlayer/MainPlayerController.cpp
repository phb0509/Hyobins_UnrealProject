// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerController.h"


AMainPlayerController::AMainPlayerController():
	m_TeamID(FGenericTeamId(1))
{

}

void AMainPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMainPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	UE_LOG(LogTemp, Warning, TEXT("MainPlayer OnPossess!"));
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return m_TeamID;
}
