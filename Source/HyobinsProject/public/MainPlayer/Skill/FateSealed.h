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
	void attackNextTarget();
	void moveToTarget();
	void OnAttackMontageEnded();
	void finishSkill();
	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> m_FateSealedMontage;
	
	UPROPERTY(EditAnywhere, Category = "Skill Settings")
	float m_SkillRange;

	UPROPERTY(EditAnywhere, Category = "Post Processing")
	TObjectPtr<UMaterialInstance> m_PostProcessingMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Finish Sound")
    TObjectPtr<USoundWave> m_FinishSound;

	TArray<TWeakObjectPtr<AMonster>> m_TargetMonsters;
	int32 m_CurTargetIndex;
	
	TWeakObjectPtr<UBattleManager> m_BattleManager;
};
