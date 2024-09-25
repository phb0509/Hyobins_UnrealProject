 //Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"
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

void AMonster::execEvent_CommonCrowdControl(const ACharacterBase* instigator)
{
	ACharacterBase* nonConstInstigator = const_cast<ACharacterBase*>(instigator);
	
	if (!m_bIsSuperArmor) // 슈퍼아머상태면 피격모션을 재생안시킬것이기 때문에 예외.
	{
		 m_AIControllerBase->StopBehaviorTree();
		 m_AIControllerBase->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, nonConstInstigator);
	}
}


void AMonster::Initialize()
{
	// HPBar 위젯 생성 및 부착.
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateMonsterHPBar(this, m_StatComponent, GetMesh(), "UpperHPBar_Widget",
		FVector(0.0f, 0.0f, 150.0f), FVector2D(150.0f, 50.0f));
}

void AMonster::Activate()
{
	m_StatComponent->InitHP();
	m_bIsActivated = true;
	m_bIsDead = false;
	m_AIControllerBase->OnPossess(this);
	m_AIControllerBase->StartBehaviorTree();
	
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), 1.0f);

	// 충돌체들 활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);

	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->BindActorToComboWidget(this);
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->BindActorToDamageWidget(this);
	this->OnTakeDamage.AddUObject(this, &AMonster::activateHitEffect);
}

void AMonster::DeActivate() // 액터풀에서 첫생성하거나 사망 후 회수되기 직전에 호출.
{
	m_bIsActivated = false;
	m_AIControllerBase->OnUnPossess();
	
	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	
	// 충돌체 비활성화.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	this->OnTakeDamage.Clear();
}

void AMonster::activateHitEffect(const FHitInformation& hitInfo)
{
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRedRatioOnHit"), 5.0f);

	this->GetWorldTimerManager().ClearTimer(m_DiffuseRatioOnHitTimer);
	this->GetWorldTimerManager().SetTimer(m_DiffuseRatioOnHitTimer,
		[this]()
		{
			GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRedRatioOnHit"), 1.0f);
		},
		0.25f,false
		);
}

