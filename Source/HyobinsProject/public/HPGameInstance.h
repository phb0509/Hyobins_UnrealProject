// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Utility/CustomStructs.h" 
#include "HPGameInstance.generated.h"



USTRUCT(BlueprintType)
struct FHPAttackInformationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHPAttackInformationData() : damage(-1.0f), bIsDot(false), bHasCrowdControl(false), crowdControlType("None"), crowdControlTime(-1.0f), bHasKnockBack(false), knockBackTime(-1.0f), knockBackDistance(-1.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bIsDot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bHasCrowdControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FName crowdControlType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float crowdControlTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bHasKnockBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float knockBackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float knockBackDistance;
};


UCLASS()
class HYOBINSPROJECT_API UHPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHPGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

	void InitAttackInformations(IN FString assetPath, OUT TMap<FName, FAttackInfoStruct>& attackInformations);

private:
	TMap<FName, UEnum*> m_Enums;

};
