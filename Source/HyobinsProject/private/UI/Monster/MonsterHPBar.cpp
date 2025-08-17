// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Monster/MonsterHPBar.h"
#include "Components/ProgressBar.h"
#include "Component/StatComponent.h"
#include "Components/Image.h"
#include "SubSystems/UIManager.h"


void UMonsterHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_GroggyTime = 6.0f;
	m_HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_HPProgressBar")));
	m_HPBarBorder= Cast<UImage>(GetWidgetFromName(TEXT("m_HPBarBorder")));

	if (m_HPProgressBarMaterialInstanceDynamic == nullptr)
	{
		UMaterialInterface* materialInterface = Cast<UMaterialInterface>(m_HPProgressBar->WidgetStyle.FillImage.GetResourceObject());
		m_HPProgressBarMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(materialInterface, this);
		m_HPProgressBar->WidgetStyle.FillImage.SetResourceObject(m_HPProgressBarMaterialInstanceDynamic.Get());
	}
	
	m_GuardRegainProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("m_GuardRegainProgressBar")));

	FSlateBrush brush = m_HPBarBorder->Brush; 
	m_HPBarBorderTexture2D = Cast<UTexture2D>(brush.GetResourceObject());

	InitHPBar();
}

void UMonsterHPBar::BindStatComponent(UStatComponent* ownerStatComponent)
{
	m_OwnerStatComponent = ownerStatComponent;

	m_OwnerStatComponent->OnDamagedHP.AddUObject(this, &UMonsterHPBar::OnDamagedHPBar);
	m_OwnerStatComponent->OnRecoveredHP.AddUObject(this, &UMonsterHPBar::OnRecoveredHPBar);
	m_OwnerStatComponent->OnStaminaIsZero.AddUObject(this, &UMonsterHPBar::OnStaminaIsZero);

	m_GroggyTime = m_OwnerStatComponent->GetGroggyTime() - 1.3f <= 0.0f ? 0.0f : m_OwnerStatComponent->GetGroggyTime() - 1.3f;
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
			//startWhiteBlink();
			UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
			
			if (uiManager != nullptr)
			{
				uiManager->StartHPBarWhiteBlink(this);
			}
			
		},
		1.3f,
		false);
	
}

void UMonsterHPBar::startWhiteBlink()
{
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

			if (m_WhiteBlinkLerpAlpha <= 0.0f && m_WhiteBlnkStopAccumulatedTime >= m_GroggyTime) // 그로기 끝나더라도 깜박임 0.0f까지는 유지되게
			{
				m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), 0.0f);
				GetWorld()->GetTimerManager().ClearTimer(m_WhiteBlinkTimer);
			}
		},
		GetWorld()->GetDeltaSeconds(),
		true);
}

void UMonsterHPBar::InitWhiteBlink()
{
	m_WhiteBlinkLerpAlpha = 0.0f;
	m_WhiteBlnkStopAccumulatedTime = 0.0f;
	m_WhiteBlinkDirection = 1;
}

bool UMonsterHPBar::WhiteBlinkTick(float deltaSeconds)
{
	m_WhiteBlinkLerpAlpha += deltaSeconds * m_WhiteBlinkSpeed * m_WhiteBlinkDirection;
	m_WhiteBlnkStopAccumulatedTime += deltaSeconds;

	if (m_WhiteBlinkLerpAlpha > 1.0f || m_WhiteBlinkLerpAlpha < 0.0f)
	{
		m_WhiteBlinkDirection *= -1;
	}

	m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), m_WhiteBlinkLerpAlpha);

	if (m_WhiteBlinkLerpAlpha <= 0.0f && m_WhiteBlnkStopAccumulatedTime >= m_GroggyTime)
	{
		m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), 0.0f);
		
		return false; // 그로기 끝나서 UIManager에서 제거.
	}
	
	return true; // 계속 업데이트.
}