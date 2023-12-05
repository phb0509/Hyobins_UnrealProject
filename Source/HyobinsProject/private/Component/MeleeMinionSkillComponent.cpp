// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MeleeMinionSkillComponent.h"

UMeleeMinionSkillComponent::UMeleeMinionSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;

}

void UMeleeMinionSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}