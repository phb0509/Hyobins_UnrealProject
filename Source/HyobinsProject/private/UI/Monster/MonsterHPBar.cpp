// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Monster/MonsterHPBar.h"
#include "Components/ProgressBar.h"
#include "Component/StatComponent.h"
#include "Components/Image.h"
#include "SubSystems/UIManager.h"


void UMonsterHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	m_WhiteBlinkTime = 6.0f;
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

	m_GroggyAnimationTime = m_GroggyAnimation->GetEndTime();

	FWidgetAnimationDynamicEvent endDelegate;
	endDelegate.BindDynamic(this, &UMonsterHPBar::onGroggyAnimationEnded);
	BindToAnimationFinished(m_GroggyAnimation, endDelegate);
	
	InitHPBar();
}

void UMonsterHPBar::NativeDestruct()
{
	Super::NativeDestruct();

	UnbindAllFromAnimationFinished(m_GroggyAnimation);
}

void UMonsterHPBar::BindStatComponent(UStatComponent* ownerStatComponent)
{
	m_OwnerStatComponent = ownerStatComponent;

	m_OwnerStatComponent->OnDamagedHP.AddUObject(this, &UMonsterHPBar::OnDamagedHPBar);
	m_OwnerStatComponent->OnRecoveredHP.AddUObject(this, &UMonsterHPBar::OnRecoveredHPBar);
	m_OwnerStatComponent->OnStaminaIsZero.AddUObject(this, &UMonsterHPBar::OnStaminaIsZero);
	
	m_WhiteBlinkTime = m_OwnerStatComponent->GetGroggyTime() - m_GroggyAnimationTime <= 0.0f ? 0.0f : m_OwnerStatComponent->GetGroggyTime() - m_GroggyAnimationTime;
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
	m_HPBarBorder->SetBrushFromMaterial(m_HPBarBorderMaterialInstance); // Border 하얗게
	PlayAnimation(m_GroggyAnimation); // 커지며 흔들리는 애니메이션
	
	// FTimerHandle timer;
	// GetWorld()->GetTimerManager().SetTimer
	// (
	// 	timer,
	// 	[this]()
	// 	{
	// 		m_HPBarBorder->SetBrushFromTexture(m_HPBarBorderTexture2D.Get()); // Border 원래 텍스쳐로 복구
	// 		
	// 		//startWhiteBlink();
	// 		UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	// 		
	// 		if (uiManager != nullptr)
	// 		{
	// 			uiManager->RegistWhiteBlinkTick(this);
	// 		}
	// 		
	// 	},
	// 	m_GroggyAnimationTime,
	// 	false);
	
}

void UMonsterHPBar::onGroggyAnimationEnded()
{
	m_HPBarBorder->SetBrushFromTexture(m_HPBarBorderTexture2D.Get()); // Border 원래 텍스쳐로 복구
			
	//startWhiteBlink();
	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
			
	if (uiManager != nullptr)
	{
		uiManager->RegistWhiteBlinkTick(this);
	}
}


void UMonsterHPBar::startWhiteBlink()
{
	InitWhiteBlink();
	
	GetWorld()->GetTimerManager().SetTimer
	(
		m_WhiteBlinkTimer,
		[=]()
		{
			//float deltaSeconds = GetWorld()->GetDeltaSeconds();
			float deltaSeconds = 0.03f;
			
			m_WhiteBlinkLerpAlpha += deltaSeconds * m_WhiteBlinkSpeed * m_WhiteBlinkDirection;
			m_WhiteBlnkStopAccumulatedTime += deltaSeconds;
	
			if (m_WhiteBlinkLerpAlpha > 1.0f || m_WhiteBlinkLerpAlpha < 0.0f)
			{
				m_WhiteBlinkDirection *= -1; // 증감방향전환.
			}
			
			m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), m_WhiteBlinkLerpAlpha);
			
			if (m_WhiteBlnkStopAccumulatedTime >= m_WhiteBlinkTime) 
			{
				m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), 0.0f);
				GetWorld()->GetTimerManager().ClearTimer(m_WhiteBlinkTimer);
			}
		},
		0.03f,
		true);
}



void UMonsterHPBar::InitWhiteBlink()
{
	m_WhiteBlinkLerpAlpha = 0.0f;
	m_WhiteBlnkStopAccumulatedTime = 0.0f;
	m_WhiteBlinkDirection = 1;
}

bool UMonsterHPBar::WhiteBlinkTick()
{
	m_WhiteBlinkLerpAlpha += 0.03f * m_WhiteBlinkSpeed * m_WhiteBlinkDirection;
	m_WhiteBlnkStopAccumulatedTime += 0.03f;

	if (m_WhiteBlinkLerpAlpha > 1.0f || m_WhiteBlinkLerpAlpha < 0.0f)
	{
		m_WhiteBlinkDirection *= -1;
	}

	m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), m_WhiteBlinkLerpAlpha);
	
	if (m_WhiteBlnkStopAccumulatedTime >= m_WhiteBlinkTime)
	{
		m_HPProgressBarMaterialInstanceDynamic->SetScalarParameterValue(TEXT("LerpAlpha"), 0.0f);
		
		return false; // 그로기 끝나서 UIManager에서 제거.
	}
	
	return true; // 계속 업데이트.
}