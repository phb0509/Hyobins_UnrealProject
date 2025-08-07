// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Monster/MonsterHPBar.h"
#include "Components/ProgressBar.h"
#include "Component/StatComponent.h"
#include "Components/Image.h"


void UMonsterHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBar")));

	if (m_HPProgressBarMaterialInstanceDynamic == nullptr)
	{
		UMaterialInterface* materialInterface = Cast<UMaterialInterface>(m_HPProgressBar->WidgetStyle.FillImage.GetResourceObject());
		m_HPProgressBarMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(materialInterface, this);
		m_HPProgressBar->WidgetStyle.FillImage.SetResourceObject(m_HPProgressBarMaterialInstanceDynamic.Get());
	
		//m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), 0.0f);
	}
	
	m_HPProgressBar->SetPercent(1.0f);
	
	m_GuardRegainProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_GuardRegainProgressBar")));

	FSlateBrush brush = m_HPBarBorder->Brush; 
	m_HPBarBorderTexture2D = Cast<UTexture2D>(brush.GetResourceObject());
	
}

void UMonsterHPBar::BindStatComponent(UStatComponent* statComponent)
{
	m_OwnerStatComponent = statComponent;

	m_OwnerStatComponent->OnDamagedHP.AddUObject(this, &UMonsterHPBar::OnDamagedHPBar);
	m_OwnerStatComponent->OnRecoveredHP.AddUObject(this, &UMonsterHPBar::OnRecoveredHPBar);
	m_OwnerStatComponent->OnStaminaIsZero.AddUObject(this, &UMonsterHPBar::OnStaminaIsZero);
}

void UMonsterHPBar::InitHPBar()
{
	m_HPProgressBar->SetPercent(1.0f);
	m_GuardRegainProgressBar->SetPercent(1.0f);
}

void UMonsterHPBar::OnDamagedHPBar()
{
	if (m_OwnerStatComponent.IsValid())
	{
		m_HPProgressBar->SetPercent(m_OwnerStatComponent->GetHPRatio());
		
		if (!GetWorld()->GetTimerManager().IsTimerActive(m_HPBarTimer)) 
		{
			GetWorld()->GetTimerManager().SetTimer(
				m_HPBarTimer,
				this,&UMonsterHPBar::updateGuardRegainProgressBar,
				GetWorld()->DeltaTimeSeconds,
				true,
				m_DecreaseSpeedFirstDelay
			);
		}
	}
}

void UMonsterHPBar::OnRecoveredHPBar() // 틱마다 호출.
{
	m_HPProgressBar->SetPercent(m_OwnerStatComponent->GetHPRatio());

	if (m_HPProgressBar->Percent >= m_GuardRegainProgressBar->Percent)
	{
		m_GuardRegainProgressBar->SetPercent(m_HPProgressBar->Percent);
	}
}

void UMonsterHPBar::updateGuardRegainProgressBar()
{
	float updatedPercent = m_GuardRegainProgressBar->Percent - m_DecreaseSpeed;

	if (updatedPercent <= m_HPProgressBar->Percent)
	{
		m_GuardRegainProgressBar->SetPercent(m_HPProgressBar->Percent);
		GetWorld()->GetTimerManager().ClearTimer(m_HPBarTimer);
	}

	m_GuardRegainProgressBar->SetPercent(updatedPercent);
}

void UMonsterHPBar::OnStaminaIsZero()
{
	// Groggy
	PlayAnimation(m_GroggyAnimation); // 커지며 흔들리는 애니메이션
	
	m_HPBarBorder->SetBrushFromMaterial(m_HPBarBorderMaterialInstance); // Border 하얗게
	
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer
	(
		timer,
		[this]()
		{
			m_HPBarBorder->SetBrushFromTexture(m_HPBarBorderTexture2D.Get()); // Border 원래 텍스쳐로 복구
			startWhiteBlink(); // 
		},
		1.0f,
		false);
	
}

void UMonsterHPBar::startWhiteBlink()
{
	// ProgressBar 하얗게 깜박
	const float groggyTime = m_OwnerStatComponent->GetGroggyTime() - 1.0f <= 0.0f ? 0.0f : m_OwnerStatComponent->GetGroggyTime() - 1.0f;
	
	m_WhiteBlinkLerpAlpha = 0.0f;
	m_WhiteBlnkStopAccumulatedTime = 0.0f;
	m_WhiteBlinkDirection = 1;
	
	GetWorld()->GetTimerManager().SetTimer
	(
		m_WhiteBlinkTimer,
		[=]()
		{
			float deltaSeconds = GetWorld()->GetDeltaSeconds();
			m_WhiteBlinkLerpAlpha += deltaSeconds * m_WhiteBlinkSpeed * m_WhiteBlinkDirection;
			m_WhiteBlnkStopAccumulatedTime += deltaSeconds;

			if (m_WhiteBlinkLerpAlpha > 1.0f || m_WhiteBlinkLerpAlpha < 0.0f)
			{
				m_WhiteBlinkDirection *= -1; // 증감방향전환.
			}
			
			m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), m_WhiteBlinkLerpAlpha);

			if (m_WhiteBlnkStopAccumulatedTime >= groggyTime)
			{
				m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), 0.0f);
				GetWorld()->GetTimerManager().ClearTimer(m_WhiteBlinkTimer);
			}
		},
		GetWorld()->GetDeltaSeconds(),
		true);
}


void UMonsterHPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
	FString log = "LerpAlpha : " + FString::SanitizeFloat(m_WhiteBlinkLerpAlpha);
	GEngine->AddOnScreenDebugMessage(500, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log));

	FString log1 = "Time : " + FString::SanitizeFloat(m_WhiteBlnkStopAccumulatedTime);
	GEngine->AddOnScreenDebugMessage(501, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log1));
}