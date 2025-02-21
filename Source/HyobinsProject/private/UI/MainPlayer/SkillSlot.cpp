// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainPlayer/SkillSlot.h"
#include "MainPlayer/Skill/Skill.h"
#include "Components/ProgressBar.h"


void USkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	m_ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_ProgressBar")));
}

void USkillSlot::SetSkill(USkill* skill, FMargin paddingValue)
{
	m_Skill = skill;
	m_Skill->OnExecute.AddUObject(this, &USkillSlot::StartCooldownUpdate);

	UTexture2D* backgroundTexture = m_Skill->GetThumbnailBackgroundTexture();
	if (backgroundTexture != nullptr)
	{
		FSlateBrush backgroundImageBrush;
		backgroundImageBrush.SetResourceObject(backgroundTexture); 
		backgroundImageBrush.ImageSize = FVector2D(100.0f, 100.0f); 

		m_ProgressBar->WidgetStyle.SetBackgroundImage(backgroundImageBrush);
	}
	
	UTexture2D* fillTextrue = m_Skill->GetThumbnailFillTexture();
	if (fillTextrue != nullptr)
	{
		FSlateBrush fillImageBrush;
		fillImageBrush.SetResourceObject(fillTextrue); 
		fillImageBrush.ImageSize = FVector2D(100.0f, 100.0f); 

		m_ProgressBar->WidgetStyle.SetFillImage(fillImageBrush);
	}
}

void USkillSlot::StartCooldownUpdate()
{
	m_CurAccumulatedTime = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(
			m_CooldownTimer, 
			this, 
			&USkillSlot::LoopCooldownUpdate, 
			GetWorld()->DeltaTimeSeconds,  // 호출 간격
			true     // 반복 여부
		);
}

void USkillSlot::LoopCooldownUpdate()
{
	const float cooldownTime = m_Skill->GetCoolDownTime();
	
	m_CurAccumulatedTime += GetWorld()->DeltaTimeSeconds;
	m_ProgressBar->SetPercent(m_CurAccumulatedTime / cooldownTime);
	
	if (m_CurAccumulatedTime >= cooldownTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_CooldownTimer);

		
	}
}
