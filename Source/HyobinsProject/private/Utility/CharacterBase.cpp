// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AIControllerBase.h"

ACharacterBase::ACharacterBase() :
	m_MaxHP(100.0f),
	m_CurHP(m_MaxHP)
{
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACharacterBase::LoadMesh(FString assetPath)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		mesh(*assetPath);

	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
}

void ACharacterBase::LoadAnimInstance(FString assetPath)
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		animInstance(*assetPath);

	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
}
