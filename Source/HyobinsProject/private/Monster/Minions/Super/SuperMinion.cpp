// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAnim.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Utility/EnumTypes.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include "Utility/Utility.h"

int ASuperMinion::TagCount(0);

ASuperMinion::ASuperMinion() :
	m_CurState(ENormalMinionStates::Patrol)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ASuperMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(FName("SuperMinion" + FString::FromInt(++TagCount)));

	m_NormalAttackSpeed = 1.0f;
	m_HitRecovery = 1.0f;
	m_PatrolRange = 500.0f;
	m_DeathTimerTime = 3.0f;

	initAssets();
}

void ASuperMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("SuperMinion :: PostInitializeComponents"));
	
	m_AnimInstance = Cast<USuperMinionAnim>(GetMesh()->GetAnimInstance());
	if (m_AnimInstance.IsValid())
	{
		m_AnimInstance->OnMontageStarted.AddDynamic(this, &ASuperMinion::SkillMontageStarted);
		m_AnimInstance->OnMontageEnded.AddDynamic(this, &ASuperMinion::SkillMontageEnded);
		
		m_AnimInstance->OnEndedNormalAttack.AddUObject(this, &ASuperMinion::OnCalledNotify_EndedNormalAttack);
		m_AnimInstance->OnEndedDeath.AddUObject(this, &ACharacterBase::OnCalledNotify_EndedDeath);
	}
	
	m_OwnerAIController = Cast<ASuperMinionAIController>(m_AIControllerBase);
}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASuperMinion::OnCalledNotify_EndedNormalAttack()
{
	m_OwnerAIController->StartBehaviorTree();
}

void ASuperMinion::ExecOnHitEvent(ACharacterBase* instigator)
{
	Super::ExecOnHitEvent(instigator);
	
	if (!m_bIsSuperArmor)
	{
		SetState(ENormalMinionStates::Hit);
		m_AnimInstance->StopAllMontages(0.0f); // OnHit이 퍼즈걸려
		m_AnimInstance->PlayMontage("OnHit_OnGround",1.0f);
		m_AnimInstance->JumpToMontageSection("OnHit_OnGround", m_HitDirection);
	}
}

void ASuperMinion::OnCalledTimer_OnHit() // 넉백시간 끝날때마다 호출.
{
	if (m_bIsDead)
	{
		GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
		return;
	}

	m_AnimInstance->StopAllMontages(0.0f);
	SetState(ENormalMinionStates::Chase);
	GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
}

void ASuperMinion::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("ASuperMinion :: Die"));

	SetState(ENormalMinionStates::Dead);
	m_OwnerAIController->GetBlackboardComponent()->SetValueAsObject(AMonster::EnemyKey, nullptr);
	m_OwnerAIController->StopBehaviorTree();
	
	m_AnimInstance->StopAllMontages(0.0f); // OnHit이 퍼즈걸려
	m_AnimInstance->PlayMontage("Death_OnGround");
	m_AnimInstance->JumpToMontageSection("Death_OnGround", m_HitDirection);
	
	for (UShapeComponent* const collider : m_HitColliders)
	{
		collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASuperMinion::SkillMontageStarted(UAnimMontage* Montage)
{
	FName name = Montage->GetFName();
	FString log = name.ToString();

	UE_LOG(LogTemp, Warning, TEXT("StartedMontage :: %s"), *log);
}

void ASuperMinion::SkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	FName name = Montage->GetFName();
	FString log = name.ToString() + " :: " + FString::FromInt(bInterrupted);
	
	UE_LOG(LogTemp, Warning, TEXT("EndedMontage :: %s"), *log);
}

void ASuperMinion::ExecEvent_EndedDeathMontage() // 사망몽타주 재생완료시 호출.
{
	m_DeathTimerRemainingTime = m_DeathTimerTime;
	m_DeathTimerTickTime = m_DeathTimerTime / 100;
	GetWorld()->GetTimerManager().SetTimer(m_DeathTimerHandle, this, &ASuperMinion::OnCalledTimer_EndedDeathEvent, m_DeathTimerTickTime, true, 0.0f);

	// 액터풀에 반환하기위한 비활성화타이머.
	GetWorldTimerManager().SetTimer(m_DeActivateTimerHandle, this, &ASuperMinion::DeActivate, m_DeathTimerTime, true);
}

void ASuperMinion::OnCalledTimer_EndedDeathEvent()
{
	m_DeathTimerRemainingTime -= m_DeathTimerTickTime;
	m_DiffuseRatio -= m_DeathTimerTickTime;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DiffuseRatio"), m_DiffuseRatio);

	if (m_DeathTimerRemainingTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_DeathTimerHandle);
	}
}

void ASuperMinion::SetState(ENormalMinionStates state)
{
	m_CurState = state;
	m_OwnerAIController->GetBlackboardComponent()->SetValueAsEnum(AMonster::StateKey, static_cast<uint8>(state));
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
		SetState(ENormalMinionStates::Hit);
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
	UCapsuleComponent* hitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	hitCollider->SetupAttachment(RootComponent);
	hitCollider->SetCapsuleHalfHeight(60.0f);
	hitCollider->SetCapsuleRadius(60.0f);
	hitCollider->SetCollisionProfileName(TEXT("HitCollider")); // HitCollider ������
	hitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	hitCollider->SetNotifyRigidBodyCollision(false);
	hitCollider->SetGenerateOverlapEvents(true);

	m_HitColliders.Add(hitCollider);
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
	GEngine->AddOnScreenDebugMessage(20, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log));
}