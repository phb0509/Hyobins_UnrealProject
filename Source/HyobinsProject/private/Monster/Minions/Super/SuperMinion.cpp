// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Utility/Utility.h"
#include "SubSystems/DataManager.h"
#include "SubSystems/DebugManager.h"
#include "SubSystems/UIManager.h"
#include "Component/CrowdControlComponent.h"


int32 ASuperMinion::TagCount(0);

ASuperMinion::ASuperMinion()
{
	PrimaryActorTick.bCanEverTick = true; 
	//AIControllerClass = ASuperMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	Tags.Add(FName("SuperMinion" + FString::FromInt(++TagCount)));
	
	initAssets();
}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();
	
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	check(dataManager != nullptr);
	
	dataManager->LoadAttackInformation(this->GetClass(),"DataTable'/Game/DataAsset/AttackInformation_SuperMinion.AttackInformation_SuperMinion'");
	dataManager->InitHitActors(this->GetClass(),m_HitActorsByMe);
	
	UDebugManager* debugManager = GetGameInstance()->GetSubsystem<UDebugManager>();
	check(debugManager != nullptr);
	
	if (debugManager != nullptr)
	{
		debugManager->OnDebugMode.AddUObject(this, &ASuperMinion::printLog);
	}
}

void ASuperMinion::Activate()
{
	Super::Activate();

	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(uiManager != nullptr);
	
	uiManager->SetVisibilityWidgets("MonsterHPBar", ESlateVisibility::HitTestInvisible);
	
	this->SetFSMState(ESuperMinionFSMStates::Patrol);
}

void ASuperMinion::printLog()
{
	if (!this->IsActive())
	{
		return;
	}

	// 로그출력.
	const FString movementMode = GetCharacterMovement()->GetMovementName();
	FString log = TEXT("SuperMinion Mode :: ");
	log += movementMode;
	GEngine->AddOnScreenDebugMessage(100, 0.1f, FColor::Green,FString::Printf(TEXT("%s"), *log));

	FString fsmState = Utility::ConvertEnumToString(static_cast<ESuperMinionFSMStates>(m_CurFSMState));
	FString log1 = Tags[0].ToString() + " :: FSMState :: " + fsmState;
	GEngine->AddOnScreenDebugMessage(101, 0.1f, FColor::Green,FString::Printf(TEXT("%s"), *log1));

	FString crowdState = Utility::ConvertEnumToString(m_CrowdControlComponent->GetCrowdControlState());
	FString log2 = Tags[0].ToString() + " :: CrowdState :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(102, 0.1f, FColor::Green,FString::Printf(TEXT("%s"), *log2));

	FString isDead = FString::FromInt(IsDead());
	FString log3 = Tags[0].ToString() + " :: IsDead :: " + isDead;
	GEngine->AddOnScreenDebugMessage(103, 0.1f, FColor::Green,FString::Printf(TEXT("%s"), *log3));
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
	check(m_HitCollider != nullptr);
	
	m_HitCollider->SetupAttachment(GetMesh(),TEXT("spine_01"));
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("HitCollider_Monster")); 
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_HitCollider->SetNotifyRigidBodyCollision(false);
	m_HitCollider->SetGenerateOverlapEvents(true);
	
	// rotation.y(pitch), rotation.z(yaw), rotation.x(roll)
	// location.x, location.y, location. z
	// scale.x,scale.y,scale.z
	
	FTransform collisionTransform = {
		{0.0f, 0.0f, 20.0f},
		{-11.111f, 115.6134, 10.4},
		{0.375f, 1.5f, 0.625f} };
	// LeftSwordCollider
	m_LeftSwordCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftSwordCollider"));
	check(m_LeftSwordCollider != nullptr);
	
	m_LeftSwordCollider->SetWorldTransform(collisionTransform);
	m_LeftSwordCollider->SetupAttachment(GetMesh(),TEXT("weapon_l"));
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
	check(m_RightSwordCollider != nullptr);
	
	m_RightSwordCollider->SetWorldTransform(collisionTransform);
	m_RightSwordCollider->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	m_RightSwordCollider->SetCollisionProfileName(TEXT("AttackCollider_Monster")); 
	m_RightSwordCollider->SetGenerateOverlapEvents(true);
	m_RightSwordCollider->SetNotifyRigidBodyCollision(false);
	m_RightSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Colliders.Add(TEXT("HitCollider"), m_HitCollider);
	m_Colliders.Add(TEXT("LeftSwordCollider"), m_LeftSwordCollider);
	m_Colliders.Add(TEXT("RightSwordCollider"), m_RightSwordCollider);
}
