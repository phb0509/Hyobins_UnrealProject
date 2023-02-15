// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Melee/MeleeMinionAIController.h"
#include "Monster/Minions/Melee/MeleeMinion.h"
#include "Kismet/GameplayStatics.h"


AMeleeMinionAIController::AMeleeMinionAIController()
{

}

void AMeleeMinionAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);


}

void AMeleeMinionAIController::BeginPlay()
{
	Super::BeginPlay();



	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // 월드의 0번째 플레이어인덱스를 가진 플레이어의 Pawn을 가져온다.
	//SetFocus(PlayerPawn); // 설정만 해놓으면 실시간으로 바라본다.
	//SetFocus(nullptr); null 넣으면 SetFocus 풀린다. (취소)
}
