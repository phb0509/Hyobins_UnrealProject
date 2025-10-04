// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Monster/BossStatusBar.h"
#include "Component/StatComponent.h"
#include "UI/Monster/MonsterHPBar.h"

void UBossStatusBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_HPBar = Cast<UMonsterHPBar>(GetWidgetFromName(TEXT("m_HPBar")));
	check(m_HPBar != nullptr);
}

void UBossStatusBar::BindStatComponent(UStatComponent* ownerStatComponent) const
{
	m_HPBar->BindStatComponent(ownerStatComponent);
}

