// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SkillComponent.h"


USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // ����Ʈ�� �α���¿�.
	bWantsInitializeComponent = false;
}
