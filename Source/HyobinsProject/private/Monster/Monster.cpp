 //Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"
#include <Components/CapsuleComponent.h>
#include "Utility/AIControllerBase.h"
#include "SubSystems/UIManager.h"
#include "Component/StatComponent.h"

const FName AMonster::HomePosKey(TEXT("HomePos"));
const FName AMonster::PatrolPosKey(TEXT("PatrolPos"));
const FName AMonster::EnemyKey(TEXT("Enemy"));
const FName AMonster::StateKey(TEXT("State"));


AMonster::AMonster() 
{
	
}

void AMonster::BeginPlay()
{
	 Super::BeginPlay();
	
	m_AIControllerBase = Cast<AAIControllerBase>(GetController());
	
}

 void AMonster::ExecOnHitEvent(ACharacterBase* instigator) // 피격시마다 호출.
{
	m_AIControllerBase->StopBehaviorTree();
	m_AIControllerBase->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, instigator);
}

void AMonster::Initialize()
{
	// HPBar 위젯 생성 및 부착.
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateHPBarComponent(this, m_StatComponent, GetMesh(), "UpperHPBar_Widget", "/Game/UI/Monster/UI_HPBar.UI_HPBar_C",
		FVector(0.0f, 0.0f, 150.0f), FVector2D(150.0f, 50.0f));
}

void AMonster::Activate()
{
	m_StatComponent->InitHP();
	m_bIsActivated = true;
	m_bIsDead = false;
	m_AIControllerBase->OnPossess(this);
	m_AIControllerBase->StartBehaviorTree();
	
	m_DiffuseRatio = 1.0f;
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), m_DiffuseRatio);

	// 충돌체들 활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	for (UShapeComponent* const collider : m_HitColliders)
	{
		collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
}

void AMonster::DeActivate() // 액터풀에서 첫생성하거나 사망 후 회수되기 직전에 호출.
{
	m_bIsActivated = false;
	m_AIControllerBase->OnUnPossess();
	
	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	GetWorldTimerManager().ClearTimer(m_DeActivateTimerHandle);
	
	// 충돌체 비활성화.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
}

