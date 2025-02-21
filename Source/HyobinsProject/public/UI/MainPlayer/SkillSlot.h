// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlot.generated.h"

class USkill;
class UProgressBar;

UCLASS()
class HYOBINSPROJECT_API USkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetSkill(USkill* skill, FMargin paddingValue);
	void StartCooldownUpdate();
	void LoopCooldownUpdate();
	
	
private:
	TWeakObjectPtr<USkill> m_Skill;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UProgressBar> m_ProgressBar;

	FTimerHandle m_CooldownTimer;
	float m_CurAccumulatedTime;
};
