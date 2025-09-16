// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BattleManager.generated.h"

class ACharacterBase;
class UDataManager;

DECLARE_MULTICAST_DELEGATE(FOnAttack);

UCLASS()
class HYOBINSPROJECT_API UBattleManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void Attack(ACharacterBase* attacker, const FName& attackName, AActor* target, const FVector& causerLocation = {0.0f, 0.0f, 0.0f});
	
	FCollisionShape MakeCollisionShape(const UShapeComponent* sweepCollider) const;
    FCollisionObjectQueryParams MakeCollisionObjectParams(const ACharacterBase* owner) const;

private:
	const UDataManager* getDataManager();
	
public:
	FOnAttack OnPlayerAttack;
	    
private:
    TWeakObjectPtr<UDataManager> m_DataManager;
};
