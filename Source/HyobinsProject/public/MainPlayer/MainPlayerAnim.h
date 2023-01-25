// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnim.generated.h"

enum class EMainPlayerStates : uint8;

UCLASS()
class HYOBINSPROJECT_API UMainPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	UMainPlayerAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MainPlayerAnim)
		EMainPlayerStates m_CurState;

	class AMainPlayer* m_MainPlayer;
};
