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



	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // ������ 0��° �÷��̾��ε����� ���� �÷��̾��� Pawn�� �����´�.
	//SetFocus(PlayerPawn); // ������ �س����� �ǽð����� �ٶ󺻴�.
	//SetFocus(nullptr); null ������ SetFocus Ǯ����. (���)
}
