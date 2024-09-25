// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Damage.h"
#include "UI/System/Combo.h"
#include "Components/TextBlock.h"

void UDamage::NativeConstruct()
{
	Super::NativeConstruct();

	m_DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("m_DamageText")));
}

void UDamage::SetDamage(float damage)
{
	m_DamageText->SetText(FText::AsNumber(damage));
	PlayAnimation(m_DamageAnimation);
}
