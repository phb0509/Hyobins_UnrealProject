 //Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"
#include "CharacterBase/AIControllerBase.h"
#include "SubSystems/UIManager.h"
#include "Component/StatComponent.h"
#include "BehaviorTree/BlackboardComponent.h"



const FName AMonster::HomePosKey(TEXT("HomePos"));
const FName AMonster::PatrolPosKey(TEXT("PatrolPos"));
const FName AMonster::EnemyKey(TEXT("Enemy"));
const FName AMonster::FSMStateKey(TEXT("FSMState"));
const FName AMonster::CrowdControlStateKet(TEXT("CrowdControlState"));


AMonster::AMonster() :
	m_CurFSMState(0),
	m_DeathCurveFloat(nullptr),
	m_DeathDissolveCurveFloat(nullptr)
{
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	m_AIControllerBase = Cast<AAIControllerBase>(GetController());

	setTimeline();
}

void AMonster::Tick(float DeltaSeconds)
{
	 Super::Tick(DeltaSeconds);

	m_DeathTimeline.TickTimeline(DeltaSeconds);
	m_DeathDissolveTimeline.TickTimeline(DeltaSeconds);
}

ACharacterBase* AMonster::GetTarget() const
{
	return Cast<ACharacterBase>(m_AIControllerBase->GetBlackboardComponent()->GetValueAsObject(TEXT("Enemy")));
}

void AMonster::execEvent_CommonCrowdControl(AActor* instigator)
{
	if (IsValid(instigator) && !m_bIsSuperArmor) // 슈퍼아머상태면 피격모션을 재생안시킬것이기 때문에 예외.
	{
		m_AIControllerBase->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, instigator);
	}
}

void AMonster::Die()
{
	Super::Die();

	m_AIControllerBase->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, nullptr);
	m_AIControllerBase->StopBehaviorTree();
}

void AMonster::ExecEvent_EndedDeathMontage()
{
	Super::ExecEvent_EndedDeathMontage();
	
	m_DeathTimeline.Play();
}

void AMonster::OnCalledTimelineEvent_Loop_AfterDeath(float curveValue)
{
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatioOnDeath"), curveValue*2);
}

void AMonster::OnCalledTimelineEvent_End_AfterDeath()
{
	// 디졸브효과 시작.
	m_DeathDissolveTimeline.Play();
}

void AMonster::OnCalledTimelineEvent_Loop_DeathDissolve(float curveValue)
{
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), curveValue);
}

void AMonster::OnCalledTimelineEvent_End_DeathDissolve()
{
	this->Deactivate();
	
	m_DeathDissolveTimeline.SetNewTime(0.0f);
	m_DeathTimeline.SetNewTime(0.0f); // 초기화
}

 void AMonster::Initialize()
{
	// HPBar 위젯 생성 및 부착.
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateMonsterHPBar(this);
}

void AMonster::Activate()
{
	SetCrowdControlState(ECrowdControlStates::None);
	m_StatComponent->InitHP();
	m_bIsDead = false;
	m_AIControllerBase->OnPossess(this);
	m_AIControllerBase->StartBehaviorTree();
	
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), 0.0f);

	// 충돌체들 활성화
	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);

	// UI
	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->BindActorToComboWidget(this);

	this->OnTakeDamage.AddUObject(uiManager, &UUIManager::RenderDamageToScreen);
}

void AMonster::Deactivate() // 액터풀에서 첫생성하거나 사망 후 회수되기 직전에 호출.
{
	m_bIsDead = true;
	m_AIControllerBase->OnUnPossess();
	
	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	
	// 충돌체 비활성화.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	for (const auto collider : m_Colliders)
	{
		if (collider.Value.IsValid())
		{
			collider.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	//this->OnTakeDamage.Clear(); // 바인딩했던 함수들 해제.
}

 bool AMonster::IsActive()
 {
	 return !m_bIsDead;
 }

void AMonster::setTimeline()
{
	 if (m_DeathCurveFloat != nullptr)
	 {
		 m_DeathTimeline.SetLooping(false);

		 FOnTimelineFloat afterDeathTimeline_Loop;
		 afterDeathTimeline_Loop.BindDynamic(this, &AMonster::OnCalledTimelineEvent_Loop_AfterDeath);
		 m_DeathTimeline.AddInterpFloat(m_DeathCurveFloat, afterDeathTimeline_Loop);

		 FOnTimelineEvent afterDeathTimeline_End;
		 afterDeathTimeline_End.BindDynamic(this, &AMonster::OnCalledTimelineEvent_End_AfterDeath);
		 m_DeathTimeline.SetTimelineFinishedFunc(afterDeathTimeline_End);
	 }

	if (m_DeathDissolveCurveFloat != nullptr)
	{
		m_DeathDissolveTimeline.SetLooping(false);

		FOnTimelineFloat dissolveDeathTimeline_Loop;
		dissolveDeathTimeline_Loop.BindDynamic(this, &AMonster::OnCalledTimelineEvent_Loop_DeathDissolve);
		m_DeathDissolveTimeline.AddInterpFloat(m_DeathDissolveCurveFloat, dissolveDeathTimeline_Loop);

		FOnTimelineEvent dissolvTimeline_End;
		dissolvTimeline_End.BindDynamic(this, &AMonster::OnCalledTimelineEvent_End_DeathDissolve);
		m_DeathDissolveTimeline.SetTimelineFinishedFunc(dissolvTimeline_End);
	}
}

 void AMonster::PlayOnHitEffect(const FHitInformation& hitInfo)
{
	Super::PlayOnHitEffect(hitInfo);
	
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRedRatioOnHit"), 5.0f); // 바로 붉게 했다가,

	this->GetWorldTimerManager().ClearTimer(m_DiffuseRatioOnHitTimer);
	this->GetWorldTimerManager().SetTimer(m_DiffuseRatioOnHitTimer,
		[this]()
		{
			GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRedRatioOnHit"), 1.0f); // 다시 원래대로
		},
		0.25f,false
		);
}

void AMonster::SetCrowdControlState(const ECrowdControlStates state)
{
	Super::SetCrowdControlState(state);
	
	m_AIControllerBase->GetBlackboardComponent()->SetValueAsEnum(AMonster::CrowdControlStateKet, static_cast<uint8>(state));
}

void AMonster::SetFSMStateAsBehaviorTree(uint8 enumIndex) const
{
	m_AIControllerBase->GetBlackboardComponent()->SetValueAsEnum(AMonster::FSMStateKey, enumIndex);
}

