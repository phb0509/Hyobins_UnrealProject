// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/AIControllerBase.h"


AAIControllerBase::AAIControllerBase() :
	m_SightRadius(300.0f),
	m_LoseSightRadius(400.0f),
	m_PeripheralVisionHalfAngle(90.0f),
	m_AISightAge(5.0f),
	m_AILastSeenLocation(900.0f)
{
	m_BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	checkf(IsValid(m_BehaviorTreeComponent), TEXT("m_BehaviorTreeComponent is not Valid"));

	m_AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent_CreatedC++");
	checkf(IsValid(m_AIPerceptionComponent), TEXT("AIPerceptionComponent is not Valid"));
}

void AAIControllerBase::LoadBehaviorTree(FString assetPath)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject
	(*assetPath);
	if (BTObject.Succeeded())
	{
		m_BehaviorTree = BTObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BTObject"));
	}

	checkf(IsValid(m_BehaviorTree), TEXT("m_BehaviorTree is not Valid"));
}

void AAIControllerBase::LoadBlackBoard(FString assetPath)
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject
	(*assetPath);
	if (BBObject.Succeeded())
	{
		m_BlackboardData = BBObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded in load BBObject"));
	}

	checkf(IsValid(m_BlackboardData), TEXT("m_BlackboardData is not Valid"));
}
