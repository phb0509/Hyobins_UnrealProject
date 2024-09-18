// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Damage.generated.h"

class UTextBlock;

UCLASS()
class HYOBINSPROJECT_API UDamage : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetDamage(float damage) const;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
    TObjectPtr<UTextBlock> m_DamageText;
};
