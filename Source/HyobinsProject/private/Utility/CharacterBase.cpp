// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"

ACharacterBase::ACharacterBase() :
	m_MaxHP(100.0f),
	m_CurHP(m_MaxHP),
	m_CurSpeed(0.0f),
	m_WalkSpeed(200.0f),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsInAir(false)
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
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
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		animInstance(*assetPath);

	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
}
