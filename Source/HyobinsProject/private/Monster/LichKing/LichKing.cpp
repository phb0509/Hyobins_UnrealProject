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


ALichKing::ALichKing()
{
	PrimaryActorTick.bCanEverTick = false; // 로그출력용.
	AIControllerClass = ALichKingAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(FName("LichKing"));
	
	initAssets();
}

void ALichKing::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInstance = Cast<ULichKingAnim>(GetMesh()->GetAnimInstance());
	m_AIController = Cast<ALichKingAIController>(GetController());

	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	dataManager->LoadAttackInformation(this->GetClass(),"DataTable'/Game/DataAsset/AttackInformation_LichKing.AttackInformation_LichKing'");
	dataManager->InitHitActors(this->GetClass(),m_HitActorsByMe);

	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->CreateBossStatusBar(this->m_StatComponent, this);

	UDebugManager* debugManager = GetGameInstance()->GetSubsystem<UDebugManager>();
	if (debugManager != nullptr)
	{
		debugManager->OnDebugMode.AddUObject(this, &ALichKing::printLog);
	}
}

void ALichKing::OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation* AttackInformation, AActor* instigator, const FVector& causerLocation)
{
	Super::OnDamage(damage, bIsCriticalAttack, AttackInformation, instigator, causerLocation);

	if (m_StatComponent->CanRecoveryStamina())
	{
		m_StatComponent->OnDamageStamina(damage * 0.01f);
	}
}

void ALichKing::Activate()
{
	Super::Activate();

	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->SetVisibilityWidgets("BossStatusBar", ESlateVisibility::HitTestInvisible);
	SetFSMState(static_cast<uint8>(ELichKingFSMStates::Chase));
}

void ALichKing::Deactivate()
{
	Super::Deactivate();
	
	GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>()->SetVisibilityWidgets("BossStatusBar", ESlateVisibility::Collapsed);
}

void ALichKing::ExecEvent_TakeAirborneAttack(AActor* instigator, const FAttackInformation* attackInfo)
{
	Super::ExecEvent_TakeKnockbackAttack(instigator, attackInfo);
}

void ALichKing::ExecEvent_TakeDownAttack(AActor* instigator, const FAttackInformation* attackInfo)
{
	Super::ExecEvent_TakeKnockbackAttack(instigator, attackInfo);
}

void ALichKing::ExecEvent_OnStaminaIsZero()
{
	Super::ExecEvent_OnStaminaIsZero();
	
	SetFSMState(ELichKingFSMStates::Groggy);
	m_AIController->RestartBehaviorTree();
}

void ALichKing::ExecEvent_EndedDeathMontage()
{
	Super::ExecEvent_EndedDeathMontage();

	UE_LOG(LogTemp, Warning, TEXT("ALichKing :: ExecEvent_EndedDeathMontage"));

	//GetMesh()->SetSimulatePhysics(true);
	

	//
	// FTimerHandle timer;
	// GetWorldTimerManager().SetTimer(timer,
	// 			[this]()
	// 			{
	// 				FVector newLocation = GetActorLocation();
	// 				newLocation.Z -= 1.0;
	// 				this->SetActorLocation(newLocation);
	// 			},
	// 		0.1f,
	// 		true);
}

void ALichKing::Die()
{
	Super::Die();

	UCapsuleComponent* rootComponent = Cast<UCapsuleComponent>(GetRootComponent());
	rootComponent->SetCapsuleHalfHeight(10.0f);
	rootComponent->SetCapsuleRadius(10.0f);
}


void ALichKing::initAssets()
{
	m_MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	
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
	m_HitCollider->SetupAttachment(GetMesh(), TEXT("spine_01"));
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("HitCollider_Monster")); 
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_HitCollider->SetNotifyRigidBodyCollision(false);
	m_HitCollider->SetGenerateOverlapEvents(true);

	m_Colliders.Add(HitColliderName, m_HitCollider);



	
	FTransform collisionTransform = {
    		{0.0f, 0.0f, 20.0f},
    		{-11.111f, 115.6134, 10.4},
    		{0.375f, 1.5f, 0.625f} };
	// LeftSwordCollider
	m_HammerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HammerCollider"));
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
 
	FString crowdState = Utility::ConvertEnumToString(m_CurCrowdControlState);
	FString log2 = Tags[0].ToString() + " :: CrowdState :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(303, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log2));
	
	const FString bIsSuperArmor = FString::FromInt(m_bIsSuperArmor);
	FString log3 = Tags[0].ToString() + " :: Is SuperArmor :: " + bIsSuperArmor;
	GEngine->AddOnScreenDebugMessage(304, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log3));
	
	GEngine->AddOnScreenDebugMessage(305, 3.f, FColor::Green, FString::Printf(TEXT("CurSpeed : %f  "), m_CurSpeed));
	GEngine->AddOnScreenDebugMessage(306, 3.f, FColor::Green, FString::Printf(TEXT("MaxWalkSpeed : %f  "), GetCharacterMovement()->MaxWalkSpeed));
}
