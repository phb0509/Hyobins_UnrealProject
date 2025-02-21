// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinion.h"
#include "Monster/Minions/Super/SuperMinionAIController.h"
#include "Utility/EnumTypes.h"
#include "Utility/Utility.h"
#include "SubSystems/DataManager.h"
#include "SubSystems/UIManager.h"

int32 ASuperMinion::TagCount(0);
const FName ASuperMinion::LeftSwordColliderName = "LeftSwordCollider";
const FName ASuperMinion::RightSwordColliderName = "RightSwordCollider";


ASuperMinion::ASuperMinion()
{
	PrimaryActorTick.bCanEverTick = true; // 로그출력용.
	AIControllerClass = ASuperMinionAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	Tags.Add(FName("SuperMinion" + FString::FromInt(++TagCount)));
	
	initAssets();
}

void ASuperMinion::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ASuperMinion :: BeginPlay"));
	
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	dataManager->LoadAttackInformation(this->GetClass(),"DataTable'/Game/DataAsset/AttackInformation_SuperMinion.AttackInformation_SuperMinion'");
	dataManager->InitHitActors(this->GetClass(),m_HitActorsByMe);
}

void ASuperMinion::Activate()
{
	Super::Activate();

	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->SetVisibilityWidgets("MonsterHPBar",this, ESlateVisibility::HitTestInvisible);
	SetFSMState(ESuperMinionFSMStates::Patrol);
}

void ASuperMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	m_DeathTimeline.TickTimeline(DeltaTime);

	// 로그출력.
	const FString movementMode = GetCharacterMovement()->GetMovementName();
	FString log = TEXT("SuperMinion Mode :: ");
	log += movementMode;
	GEngine->AddOnScreenDebugMessage(100, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log));
	
	FString fsmState = Utility::ConvertEnumToString(static_cast<ESuperMinionFSMStates>(m_CurFSMState));
	FString log1 = Tags[0].ToString() + " :: FSMState :: " + fsmState;
	GEngine->AddOnScreenDebugMessage(104, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log1));
 
	FString crowdState = Utility::ConvertEnumToString(m_CurCrowdControlState);
    FString log2 = Tags[0].ToString() + " :: CrowdState :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(105, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log2));
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
	m_RightSwordCollider->SetWorldTransform(collisionTransform);
	m_RightSwordCollider->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	m_RightSwordCollider->SetCollisionProfileName(TEXT("AttackCollider_Monster")); 
	m_RightSwordCollider->SetGenerateOverlapEvents(true);
	m_RightSwordCollider->SetNotifyRigidBodyCollision(false);
	m_RightSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Colliders.Add(HitColliderName, m_HitCollider);
	m_Colliders.Add(LeftSwordColliderName, m_LeftSwordCollider);
	m_Colliders.Add(RightSwordColliderName, m_RightSwordCollider);
}