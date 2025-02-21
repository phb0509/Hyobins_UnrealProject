// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlots.generated.h"

class USkillSlot;
class USkillComponent;
class UImage;
class UHorizontalBox;


USTRUCT(Atomic) 
struct FSkillSlotInfos
{
	GENERATED_USTRUCT_BODY() 

public:
	UPROPERTY()
	TArray<TObjectPtr<USkillSlot>> skillSlots;
};

UCLASS()
class HYOBINSPROJECT_API USkillSlots : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void CreateSkillListFromSkillComponent(USkillComponent* skillComponent);
	void ChangeSkillSlots();
	

private:
	FName m_CurModeName;
	TWeakObjectPtr<USkillComponent> m_SkillComponent;
	
	UPROPERTY()
	TMap<FName, FSkillSlotInfos> m_SkillSlots;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UImage> m_BackGroundImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UHorizontalBox> m_HorizontalBox;


	UPROPERTY(EditAnywhere, Category = "SkillSlot Padding")
	float m_LeftPadding;

	UPROPERTY(EditAnywhere, Category = "SkillSlot Padding")
	float m_RightPadding;

	UPROPERTY(EditAnywhere, Category = "SkillSlot Padding")
	float m_TopPadding;

	UPROPERTY(EditAnywhere, Category = "SkillSlot Padding")
	float m_BottomPadding;
};
