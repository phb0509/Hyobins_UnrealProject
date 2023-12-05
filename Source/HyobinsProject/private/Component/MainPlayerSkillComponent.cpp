// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MainPlayerSkillComponent.h"

UMainPlayerSkillComponent::UMainPlayerSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;

}

void UMainPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}