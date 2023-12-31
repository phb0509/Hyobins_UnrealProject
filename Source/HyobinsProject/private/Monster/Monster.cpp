 //Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"
#include "Utility/AIControllerBase.h"
#include "Utility/AnimInstanceBase.h"
#include <Components/CapsuleComponent.h>
#include "SubSystems/UIManager.h"

const FName AMonster::HomePosKey(TEXT("HomePos"));
const FName AMonster::PatrolPosKey(TEXT("PatrolPos"));
const FName AMonster::EnemyKey(TEXT("Enemy"));
const FName AMonster::StateKey(TEXT("State"));
const FName AMonster::NormalAttackSpeedKey(TEXT("NormalAttackSpeed"));


AMonster::AMonster() :
	m_PatrolRange(1000.0f),
	m_NormalAttackRange(150.0f),
	m_NormalAttackSpeed(1.0f)
{

}

void AMonster::ExecHitEvent(ACharacterBase* instigator)
{
	m_AIControllerBase->PlayBehaviorTree();
	m_AIControllerBase->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, instigator);
}

void AMonster::Initialize()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateHPBarComponent(this, GetMesh(), "UpperHPBar_Widget", "/Game/UI/Monster/UI_HPBar.UI_HPBar_C", FVector(0.0f, 0.0f, 150.0f), FVector2D(150.0f, 50.0f));
}

void AMonster::Activate()
{
	//m_StatComponent->InitHP();
	m_bIsActivated = true;
	m_bIsDeath = false;
	m_AIControllerBase->OnPossess(this);
	m_AIControllerBase->PlayBehaviorTree();

	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	m_DiffuseRatio = 1.0f;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), m_DiffuseRatio);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	for (UShapeComponent* collider : m_HitColliders)
	{
		collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMonster::DeActivate()
{
	m_bIsActivated = false;
	m_AIControllerBase->OnUnPossess();
	m_AnimInstanceBase->StopAllMontages(0.1f);
	GetWorldTimerManager().ClearTimer(m_DeActivateTimerHandle);

	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	for (UShapeComponent* collider : m_HitColliders)
	{
		collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}