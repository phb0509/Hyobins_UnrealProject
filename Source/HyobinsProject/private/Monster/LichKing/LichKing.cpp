// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/LichKing/LichKing.h"
#include "Monster/LichKing/LichKingAIController.h"
#include "Monster/LichKing/LichKingAnim.h"
#include "MotionWarpingComponent.h"
#include "Component/StatComponent.h"
#include "Utility/Utility.h"
#include "SubSystems/DataManager.h"
#include "SubSystems/DebugManager.h"
#include "SubSystems/UIManager.h"
#include "Component/CrowdControlComponent.h"

ALichKing::ALichKing()
{
	PrimaryActorTick.bCanEverTick = false; // 로그출력용.
	AIControllerClass = ALichKingAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(TEXT("LichKing"));
	
	initAssets();
}

void ALichKing::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInstance = Cast<ULichKingAnim>(GetMesh()->GetAnimInstance());
	check(m_AnimInstance != nullptr);
	
	m_AIController = Cast<ALichKingAIController>(GetController());
	check(m_AIController != nullptr);

	// 공격정보 초기화
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	check(dataManager != nullptr);
	
	dataManager->LoadAttackInformation(this->GetClass(),"DataTable'/Game/DataAsset/AttackInformation_LichKing.AttackInformation_LichKing'");
	dataManager->InitHitActors(this->GetClass(),m_HitActorsByMe);

	// 체력바 부착
	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(uiManager != nullptr);
	
	uiManager->CreateBossStatusBar(this->m_StatComponent, this);

	// 디버깅용
	UDebugManager* debugManager = GetGameInstance()->GetSubsystem<UDebugManager>();
	check(debugManager != nullptr);
	
	if (debugManager != nullptr)
	{
		debugManager->OnDebugMode.AddUObject(this, &ALichKing::printLog);
	}
}

void ALichKing::EndedGroggy()
{
	Super::EndedGroggy();

	this->SetIsSuperArmor(true);
}

void ALichKing::Activate()
{
	Super::Activate();

	this->SetActorLocation({0.0f, 0.0f, 500.0f});

	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(uiManager != nullptr);
	
	uiManager->SetVisibilityWidgets("BossStatusBar", ESlateVisibility::HitTestInvisible);
	
	this->SetFSMState(ELichKingFSMStates::Chase);
}

void ALichKing::Deactivate()
{
	Super::Deactivate();

	UUIManager* uiManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	check(uiManager != nullptr);
	
	uiManager->SetVisibilityWidgets("BossStatusBar", ESlateVisibility::Collapsed);
}

void ALichKing::ExecEvent_EndedDeathMontage()
{
	Super::ExecEvent_EndedDeathMontage();

	this->Deactivate();
}

void ALichKing::Die()
{
	Super::Die();

	UCapsuleComponent* rootComponent = Cast<UCapsuleComponent>(GetRootComponent());
	check(rootComponent != nullptr);
	
	rootComponent->SetCapsuleHalfHeight(10.0f);
	rootComponent->SetCapsuleRadius(10.0f);
}


void ALichKing::initAssets()
{
	m_MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	check(m_MotionWarpingComponent != nullptr);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/MonsterAsset/LichKing/LichKing.LichKing'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	checkf(IsValid(mesh.Object), TEXT("Mesh is not Valid"));

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstance(TEXT("AnimBlueprint'/Game/MonsterAsset/LichKing/ABP_LichKing.ABP_LichKing_C'"));
	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
	checkf(IsValid(animInstance.Class), TEXT("AnimInstance is not Valid"));

	
	// HitCollider
	m_HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	check(m_HitCollider != nullptr);
	
	m_HitCollider->SetupAttachment(GetMesh(), TEXT("spine_01"));
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("HitCollider_Monster")); 
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_HitCollider->SetNotifyRigidBodyCollision(false);
	m_HitCollider->SetGenerateOverlapEvents(true);

	m_Colliders.Add(TEXT("HitCollider"), m_HitCollider);



	
	FTransform collisionTransform = {
    		{0.0f, 0.0f, 20.0f},
    		{-11.111f, 115.6134, 10.4},
    		{0.375f, 1.5f, 0.625f} };
	// LeftSwordCollider
	m_HammerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HammerCollider"));
	check(m_HammerCollider != nullptr);
	
	m_HammerCollider->SetWorldTransform(collisionTransform);
	m_HammerCollider->SetupAttachment(GetMesh(),TEXT("HammerCenter"));
	m_HammerCollider->SetCollisionProfileName(TEXT("AttackCollider_Monster"));
	m_HammerCollider->SetGenerateOverlapEvents(true);
	m_HammerCollider->SetNotifyRigidBodyCollision(false);
	m_HammerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	m_Colliders.Add(TEXT("HammerCollider"), m_HammerCollider);
}

void ALichKing::printLog()
{
	if (!this->IsActive()) return;
	// 로그출력.
	const FVector location = GetActorLocation();
	GEngine->AddOnScreenDebugMessage(300, 3.f, FColor::Green, FString::Printf(TEXT("LichKing Location : %f  %f  %f"), location.X, location.Y, location.Z));
	
	const FString movementMode = GetCharacterMovement()->GetMovementName();
	FString log = TEXT("LichKing Mode :: ");
	log += movementMode;
	GEngine->AddOnScreenDebugMessage(301, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log));
	
	FString fsmState = Utility::ConvertEnumToString(static_cast<ELichKingFSMStates>(m_CurFSMState));
	FString log1 = Tags[0].ToString() + " :: FSMState :: " + fsmState;
	GEngine->AddOnScreenDebugMessage(302, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log1));
 
	FString crowdState = Utility::ConvertEnumToString(m_CrowdControlComponent->GetCrowdControlState());
	FString log2 = Tags[0].ToString() + " :: CrowdState :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(303, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log2));
	
	const FString bIsSuperArmor = FString::FromInt(m_bIsSuperArmor);
	FString log3 = Tags[0].ToString() + " :: Is SuperArmor :: " + bIsSuperArmor;
	GEngine->AddOnScreenDebugMessage(304, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log3));
	
	GEngine->AddOnScreenDebugMessage(305, 3.f, FColor::Green, FString::Printf(TEXT("CurSpeed : %f  "), m_CurSpeed));
	GEngine->AddOnScreenDebugMessage(306, 3.f, FColor::Green, FString::Printf(TEXT("MaxWalkSpeed : %f  "), GetCharacterMovement()->MaxWalkSpeed));
}
