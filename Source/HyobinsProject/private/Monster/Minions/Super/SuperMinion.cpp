// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAnim.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Utility/EnumTypes.h"
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>
#include "Utility/Utility.h"

int32 ASuperMinion::TagCount(0);
const FName ASuperMinion::HitColliderName = "HitCollider";
const FName ASuperMinion::LeftSwordColliderName = "LeftSwordCollider";
const FName ASuperMinion::RightSwordColliderName = "RightSwordCollider";

ASuperMinion::ASuperMinion() :
	m_CurState(ENormalMinionStates::Patrol)
{
	PrimaryActorTick.bCanEverTick = true; // 로그출력용.
	AIControllerClass = ASuperMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	Tags.Add(FName("SuperMinion" + FString::FromInt(++TagCount)));
	
	m_DeathTimerTime = 3.0f;

	initAssets();
}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInstance = Cast<USuperMinionAnim>(GetMesh()->GetAnimInstance());
	m_AIController = Cast<ASuperMinionAIController>(GetController());

	if (m_AnimInstance != nullptr)
	{
		m_AnimInstance->BindFunc_OnMontageEnded(TEXT("NormalAttack0"),this,TEXT("OnCalled_NormalAttack_End"));
		m_AnimInstance->BindFunc_OnMontageEnded(TEXT("NormalAttack1"),this,TEXT("OnCalled_NormalAttack_End"));
	}
}

void ASuperMinion::OnCalled_NormalAttack_End()
{
	if (m_CurState == ENormalMinionStates::NormalAttack)
	{
		m_AIController->StartBehaviorTree();
	}
}

void ASuperMinion::ExecEvent_Knockback(ACharacterBase* instigator)
{
	Super::ExecEvent_Knockback(instigator);
	
	if (!m_bIsSuperArmor)
	{
		SetState(ENormalMinionStates::Knockback);
		m_AnimInstance->StopAllMontages(0.0f);
		//m_AnimInstance->PlayMontage(USuperMinionAnim::HitMontages[m_HitDirection],1.0f);
	}
}

void ASuperMinion::OnCalledTimer_Knockback_End() // 넉백시간 끝날때마다 호출.
{
	if (m_bIsDead)
	{
		GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		return;
	}

	m_AnimInstance->StopAllMontages(0.0f);
	m_AIController->StartBehaviorTree();
	SetState(ENormalMinionStates::Chase);
	GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
}

void ASuperMinion::ExecEvent_Groggy(ACharacterBase* instigator)
{
	Super::ExecEvent_Groggy(instigator);
	
	SetState(ENormalMinionStates::Groggy);
	m_AnimInstance->StopAllMontages(0.0f);
	//m_AnimInstance->PlayMontage(HitMontages[m_HitDirection],1.0f);
}

void ASuperMinion::Die()
{
	SetState(ENormalMinionStates::Dead);
	m_AIController->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, nullptr);
	m_AIController->StopBehaviorTree();
	
	m_AnimInstance->StopAllMontages(0.0f);  
	//m_AnimInstance->PlayMontage(DeathMontages[m_HitDirection]);
	
	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASuperMinion::ExecEvent_EndedDeathMontage() // 사망몽타주 재생완료시 호출.
{
	m_DeathTimerRemainingTime = m_DeathTimerTime;
	m_DeathTimerTickTime = m_DeathTimerTime / 100;
	
	GetWorldTimerManager().SetTimer(m_DeathTimerHandle, this, &ASuperMinion::onCalledTimer_EndedDeathEvent, m_DeathTimerTickTime, true, 0.0f);

	// 액터풀에 반환하기위한 비활성화타이머.
	GetWorldTimerManager().SetTimer(m_DeActivateTimerHandle, this, &ASuperMinion::DeActivate, m_DeathTimerTime, true);
}

void ASuperMinion::onCalledTimer_EndedDeathEvent()
{
	m_DeathTimerRemainingTime -= m_DeathTimerTickTime;
	m_DiffuseRatio -= m_DeathTimerTickTime;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), m_DiffuseRatio);

	if (m_DeathTimerRemainingTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_DeathTimerHandle);
	}
}

void ASuperMinion::SkillMontageStarted(UAnimMontage* Montage)
{
	FName name = Montage->GetFName();
	FString log = name.ToString();

	//UE_LOG(LogTemp, Warning, TEXT("StartedMontage :: %s"), *log);
}

void ASuperMinion::SkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	FName montageName = Montage->GetFName();
	FString log = montageName.ToString() + " :: " + FString::FromInt(bInterrupted);

	// m_AnimInstance->ExecBindedFunc_OnMontageEnded(montageName);
	// UE_LOG(LogTemp, Warning, TEXT("EndedMontage :: %s"), *log);
}

void ASuperMinion::SetState(ENormalMinionStates state)
{
	m_CurState = state;
	m_AIController->GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, static_cast<uint8>(state));
}

void ASuperMinion::SetCommonState(const int32 commonStateIndex)
{
	const UEnum* enumObject = FindObject<UEnum>(nullptr ,L"/Script/CoreUObject.Class'/Script/HyobinsProject.ENormalMinionStates'");
	ENormalMinionStates enumValue = (ENormalMinionStates)(enumObject->GetValueByIndex(commonStateIndex));

	switch (commonStateIndex)
	{
	case 0:
		SetState(ENormalMinionStates::Patrol);
		break;
		
	case 1:
		SetState(ENormalMinionStates::Chase);
		break;
	
	case 2:
		SetState(ENormalMinionStates::Knockback);
		break;
	
	case 3:
		SetState(ENormalMinionStates::Dead);
		break;

	case 4:
		SetState(ENormalMinionStates::NormalAttack);
		break;
	
	default:
		break;
	}
}

void ASuperMinion::Activate()
{
	Super::Activate();
	
	SetState(ENormalMinionStates::Patrol);
	
	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASuperMinion::DeActivate()
{
	Super::DeActivate();

	m_Colliders[HitColliderName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASuperMinion::initAssets()
{
	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/MonsterAsset/SuperMinion/Character/SuperMinion.SuperMinion'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	checkf(IsValid(mesh.Object), TEXT("Mesh is not Valid"));

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstance(TEXT("AnimBlueprint'/Game/MonsterAsset/SuperMinion/ABP_SuperMinion.ABP_SuperMinion_C'"));
	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
	checkf(IsValid(animInstance.Class), TEXT("AnimInstance is not Valid"));
	
	// HitCollider
	m_HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	m_HitCollider->SetupAttachment(RootComponent);
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("HitCollider_Monster")); 
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_HitCollider->SetNotifyRigidBodyCollision(false);
	m_HitCollider->SetGenerateOverlapEvents(true);
	

	// rotation.y(pitch), rotation.z(yaw), rotation.x(roll)
	// location.x, location.y, location. z
	// scale.x,scale.y,scale.z
	// 
	// m_ShieldForAttackCollider->SetWorldTransform(collisionTransform);


	FTransform collisionTransform = {
		{0.0f, 0.0f, 20.0f},
		{-11.111f, 115.6134, 10.4},
		{0.375f, 1.5f, 0.625f} };
	// LeftSwordCollider
	m_LeftSwordCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSwordCollider"));
	m_LeftSwordCollider->SetWorldTransform(collisionTransform);
	m_LeftSwordCollider->SetupAttachment(GetMesh(), FName(TEXT("weapon_l")));
	m_LeftSwordCollider->SetCollisionProfileName(TEXT("AttackCollider_Monster")); 
	m_LeftSwordCollider->SetGenerateOverlapEvents(true);
	m_LeftSwordCollider->SetNotifyRigidBodyCollision(false);
	m_LeftSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	collisionTransform = {
		{0.0f, 0.0f, 160.0f},
		{0.0f, -113.333f, -13.3333f},
		{0.25f, 1.5f, 0.625f} };
	
	// RightSwordCollider
	m_RightSwordCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("RightSwordCollider"));
	m_RightSwordCollider->SetWorldTransform(collisionTransform);
	m_RightSwordCollider->SetupAttachment(GetMesh(), FName(TEXT("weapon_r")));
	m_RightSwordCollider->SetCollisionProfileName(TEXT("AttackCollider_Monster")); 
	m_RightSwordCollider->SetGenerateOverlapEvents(true);
	m_RightSwordCollider->SetNotifyRigidBodyCollision(false);
	m_RightSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Colliders.Add(HitColliderName, m_HitCollider);
	m_Colliders.Add(LeftSwordColliderName, m_LeftSwordCollider);
	m_Colliders.Add(RightSwordColliderName, m_RightSwordCollider);
}

void ASuperMinion::updateState()
{
	m_CurSpeed = GetVelocity().Size();
}

void ASuperMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
	FString state = Utility::ConvertEnumToString(m_CurState);
	FString log = Tags[0].ToString() + " :: " + state;
	//GEngine->AddOnScreenDebugMessage(20, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log));
}
