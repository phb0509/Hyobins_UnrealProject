// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SkillComponent.h"


USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // 뷰포트에 로그출력용.
	bWantsInitializeComponent = false;
}
