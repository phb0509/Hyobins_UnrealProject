// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Monster/MonsterHPBar.h"
#include "BossStatusBar.generated.h"

class UStatComponent;
class UProgressBar;
class UMonsterHPBar;

UCLASS()
class HYOBINSPROJECT_API UBossStatusBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void BindStatComponent(UStatComponent* ownerStatComponent) const;
	

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
    TObjectPtr<UMonsterHPBar> m_HPBar;
    	

};
