// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeMinionSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HYOBINSPROJECT_API UMeleeMinionSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMeleeMinionSkillComponent();

protected:
	virtual void BeginPlay() override;

private:
	//TMap<FName, FAttackInfoStruct> m_AttackInformations;

};
