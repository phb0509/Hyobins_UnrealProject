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


UCLASS()
class HYOBINSPROJECT_API USkill : public UObject
{
	GENERATED_BODY()

public:
	USkill();

	virtual void Initialize() {};
	virtual void Execute();
	
	
	FORCEINLINE UTexture2D* GetThumbnailFillTexture() const { return m_ThumbnailFillTexture; }
	FORCEINLINE UTexture2D* GetThumbnailBackgroundTexture() const { return m_ThumbnailBackgroundTexture; }
	FORCEINLINE float GetCoolDownTime() const { return m_CoolDownTime; }
	
	FORCEINLINE void SetOwnerInfo(AMainPlayer* owner);
	FORCEINLINE void SetCoolDownTime(float coolDownTime) { m_CoolDownTime = coolDownTime; }

public:
	FOnExecute OnExecute;
	
protected:
	TWeakObjectPtr<AMainPlayer> m_Owner;
	TWeakObjectPtr<UMainPlayerAnim> m_OwnerAnimInstance;
	TWeakObjectPtr<UMainPlayerSkillComponent> m_OwnerSkillComponent;

	UPROPERTY(EditAnywhere, Category = "Thumbnail")
	TObjectPtr<UTexture2D> m_ThumbnailFillTexture;

	UPROPERTY(EditAnywhere, Category = "Thumbnail")
	TObjectPtr<UTexture2D> m_ThumbnailBackgroundTexture;
	
	UPROPERTY(EditAnywhere, Category = "CoolDownTime")
	float m_CoolDownTime;

	bool m_bIsCoolDownActive;
};
