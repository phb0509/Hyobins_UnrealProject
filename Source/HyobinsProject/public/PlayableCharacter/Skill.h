// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill.generated.h"

class APlayableCharacter;
class UAnimInstanceBase;
class USkillComponent;
class UImage;
class UTexture;
class UTexture2D;

DECLARE_MULTICAST_DELEGATE(FOnExecute);


UCLASS(Abstract, Blueprintable, BlueprintType)
class HYOBINSPROJECT_API USkill : public UObject
{
	GENERATED_BODY()

public:
	USkill();

	virtual void Initialize() {};
	virtual void Execute();
	
	bool GetCanUseSkill() const { return m_bIsCooldownComplete && GetCanExecuteSkill(); }
	
	FORCEINLINE FName& GetName() { return m_Name; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	FORCEINLINE float GetCoolDownTime() const { return m_CoolDownTime; }
	FORCEINLINE UTexture2D* GetThumbnailFillTexture() const { return m_ThumbnailFillTexture; }
	FORCEINLINE UTexture2D* GetThumbnailBackgroundTexture() const { return m_ThumbnailBackgroundTexture; }
	
	FORCEINLINE void SetOwnerInfo(APlayableCharacter* owner);
	FORCEINLINE void SetCoolDownTime(float coolDownTime) { m_CoolDownTime = coolDownTime; }

protected:
	virtual bool GetCanExecuteSkill() const { return false; }
	
public:
	FOnExecute OnExecute;
	
protected:
	TWeakObjectPtr<APlayableCharacter> m_Owner;
	TWeakObjectPtr<UAnimInstanceBase> m_OwnerAnimInstance;
	TWeakObjectPtr<USkillComponent> m_OwnerSkillComponent;

	FTimerHandle m_TimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "SkillName")
	FName m_Name;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float m_CoolDownTime;

	bool m_bIsCooldownComplete;
	
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float m_StaminaCost;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
	bool m_bIsSuperArmor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Thumbnail")
	TObjectPtr<UTexture2D> m_ThumbnailFillTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Thumbnail")
	TObjectPtr<UTexture2D> m_ThumbnailBackgroundTexture;
	
	


};
