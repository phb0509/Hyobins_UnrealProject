// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill.generated.h"

class AMainPlayer;
class UMainPlayerAnim;
class UMainPlayerSkillComponent;
class UImage;
class UTexture;
class UTexture2D;

DECLARE_MULTICAST_DELEGATE(FOnExecute);


UCLASS(Blueprintable, BlueprintType)
class HYOBINSPROJECT_API USkill : public UObject
{
	GENERATED_BODY()

public:
	USkill();

	virtual void Initialize() {};
	virtual void Execute();

	FORCEINLINE FName& GetSkillName() { return m_SkillName; }
	FORCEINLINE UTexture2D* GetThumbnailFillTexture() const { return m_ThumbnailFillTexture; }
	FORCEINLINE UTexture2D* GetThumbnailBackgroundTexture() const { return m_ThumbnailBackgroundTexture; }
	FORCEINLINE float GetCoolDownTime() const { return m_CoolDownTime; }
	FORCEINLINE bool GetIsSuperArmor() const { return m_bIsSuperArmor; }
	
	FORCEINLINE void SetOwnerInfo(AMainPlayer* owner);
	FORCEINLINE void SetCoolDownTime(float coolDownTime) { m_CoolDownTime = coolDownTime; }

public:
	FOnExecute OnExecute;
	
protected:
	TWeakObjectPtr<AMainPlayer> m_Owner;
	TWeakObjectPtr<UMainPlayerAnim> m_OwnerAnimInstance;
	TWeakObjectPtr<UMainPlayerSkillComponent> m_OwnerSkillComponent;

	UPROPERTY(EditDefaultsOnly, Category = "SkillName")
	FName m_SkillName;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float m_CoolDownTime;

	bool m_bIsCoolDownActive;
	
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	float m_StaminaCost;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
	bool m_bIsSuperArmor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Thumbnail")
	TObjectPtr<UTexture2D> m_ThumbnailFillTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Thumbnail")
	TObjectPtr<UTexture2D> m_ThumbnailBackgroundTexture;
	
	


};
