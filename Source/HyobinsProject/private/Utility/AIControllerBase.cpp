// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/AIControllerBase.h"


AAIControllerBase::AAIControllerBase()
{
	m_BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	checkf(IsValid(m_BehaviorTreeComponent), TEXT("m_BehaviorTreeComponent is not Valid"));
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
