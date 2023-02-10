// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"


ACharacterBase::ACharacterBase() :
	m_MaxHP(100.0f),
	m_CurHP(m_MaxHP)
{

}

void ACharacterBase::LoadMesh(FString assetPath)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempMesh(*assetPath);

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
}

void ACharacterBase::LoadAnimInstance(FString assetPath)
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MainPlayer_AnimInstance(*assetPath);

	if (MainPlayer_AnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MainPlayer_AnimInstance.Class);
	}
}
