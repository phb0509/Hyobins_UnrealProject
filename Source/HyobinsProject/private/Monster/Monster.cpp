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
const FName AMonster::NormalAttackSpeedKey(TEXT("NormalAttackSpeed"));


AMonster::AMonster() :
	m_PatrolRange(1000.0f),
	m_NormalAttackRange(150.0f),
	m_NormalAttackSpeed(1.0f)
{
}

void AMonster::ExecOnHitEvent(ACharacterBase* instigator) // 피격시마다 호출.
{
	m_AIControllerBase->StartBehaviorTree(); // 공격도중에 피격되면 계속 Stop상태니까 Start해놨네 (공격하면 강제로 정지시키니까) // 삭제예정.
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

	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	m_DiffuseRatio = 1.0f;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), m_DiffuseRatio);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	for (UShapeComponent* const collider : m_HitColliders)
	{
		collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMonster::DeActivate() // 액터풀에서 첫생성하거나 사망 후 회수되기 직전에 호출.
{
	m_bIsActivated = false;
	m_AIControllerBase->OnUnPossess();
	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	GetWorldTimerManager().ClearTimer(m_DeActivateTimerHandle);

	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	// 충돌체 비활성화.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

