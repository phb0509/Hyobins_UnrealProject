// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SuperMinion.generated.h"

class USuperMinionAnim;
class ASuperMinionAIController;
enum class ESuperMinionFSMStates : uint8;

UCLASS()
class HYOBINSPROJECT_API ASuperMinion final: public AMonster
{
	GENERATED_BODY()

public:
	ASuperMinion();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCalled_NormalAttack_End() const;

protected:
	// IPoolableActor VirtualFunction 
	virtual void Activate() override;
	//virtual void DeActivate() override;

	
private:
	void initAssets();
	void bindFuncOnMontagEvent();
	

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> m_HitCollider;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_LeftSwordCollider;
		
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_RightSwordCollider;
	
	TWeakObjectPtr<ASuperMinionAIController> m_AIController;
	TWeakObjectPtr<USuperMinionAnim> m_AnimInstance;
	
	static int32 TagCount;
	static const FName LeftSwordColliderName;
	static const FName RightSwordColliderName;
	
};
