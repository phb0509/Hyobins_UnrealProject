// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Damage.h"
//#include "UI/System/Combo.h"
#include "Components/TextBlock.h"

void UDamage::NativeConstruct()
{
	Super::NativeConstruct();

	m_DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("m_DamageText")));
	check(m_DamageText != nullptr);
	
	FWidgetAnimationDynamicEvent endDelegate;
	endDelegate.BindDynamic(this, &UDamage::Remove);
	BindToAnimationFinished(m_DamageAnimation, endDelegate);
}

void UDamage::NativeDestruct()
{
	Super::NativeDestruct();
	
	UnbindAllFromAnimationFinished(m_DamageAnimation);
}

void UDamage::SetDamage(float damage)
{
	m_DamageText->SetText(FText::AsNumber(damage));
	PlayAnimation(m_DamageAnimation);
}

void UDamage::Remove()
{
	this->RemoveFromParent();
}
