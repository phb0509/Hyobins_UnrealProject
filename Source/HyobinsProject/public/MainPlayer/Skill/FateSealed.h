// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Skill.h"
#include "FateSealed.generated.h"

class AMonster;
class UBattleManager;

UCLASS()
class HYOBINSPROJECT_API UFateSealed : public USkill
{
	GENERATED_BODY()
	
public:
	UFateSealed();

	virtual void Initialize() override;
	virtual void Execute() override;

protected:
	virtual bool CanExecuteSkill() const override;
	
private:
	void findTargets();
	void moveToNextTarget();
	void onFateSealedEnded();
	void finishSkill();
	void onFateSealedFinishEnded();
	void updateAllStencilValues();
	void executeStaggeredAttack();

	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_FateSealedMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_FateSealedFinishMontage;
	
	UPROPERTY(EditAnywhere, Category = "Skill Settings")
	float m_SkillRange;

	UPROPERTY(EditAnywhere, Category = "Post Processing")
	TObjectPtr<UMaterialInstance> m_PPMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Finish Sound")
    TObjectPtr<USoundWave> m_FinishSound;

	TArray<TWeakObjectPtr<AMonster>> m_TargetMonsters;
	int32 m_CurTargetIndex;
	
	TWeakObjectPtr<UBattleManager> m_BattleManager;

	TWeakObjectPtr<UMaterialInstanceDynamic> m_DynamicPPMaterial;

	TWeakObjectPtr<AMonster> m_CurTarget;

	FVector m_SkillStartLocation;	
	
	UPROPERTY(EditAnywhere, Category = "Skill Settings")
	float m_FinishAttackDelay;

	UPROPERTY(EditAnywhere, Category = "Skill Settings")
	float m_FinishAttackDelayGap;
	
	float m_AttackDelay;

	UPROPERTY()
	FTimerHandle m_StencilUpdateTimer;

	UPROPERTY()
	TArray<TWeakObjectPtr<AMonster>> m_StencilTargets;

	float m_FinishAttackCallTime;
	
};
