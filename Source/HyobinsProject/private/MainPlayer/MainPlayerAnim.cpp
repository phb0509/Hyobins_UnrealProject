// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"
#include "Utility/EnumTypes.h"

UMainPlayerAnim::UMainPlayerAnim():
	m_CurState(EMainPlayerStates::Idle)
{
	
}

void UMainPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(8, 3.f, FColor::Red, FString::Printf(TEXT("animInstance Log")));

	m_MainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());

}
