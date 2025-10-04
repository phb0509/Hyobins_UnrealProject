// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayerController.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Component/StatComponent.h"
#include "Utility/Utility.h"
#include "SubSystems/DataManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Component/CrowdControlComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystems/DebugManager.h"


const FName AMainPlayer::SwordColliderName = "SwordCollider";
const FName AMainPlayer::ShieldForAttackColliderName = "ShieldForAttackCollider";
const FName AMainPlayer::ShieldForGuardColliderName = "ShieldForGuardCollider";

AMainPlayer::AMainPlayer() :
	m_bIsParrying(false)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMainPlayerController::StaticClass();

	this->Tags.Add(TEXT("MainPlayer"));

	m_WalkSpeed = 300.0f;
	m_RunSpeed = 1300.0f;
	
	initAssets();
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("MainPlayer::BeginPlay"));
	
	this->SetActorLocation(FVector(0.0f, 0.0f, 40.0f));
	
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	check(dataManager != nullptr);
	
	dataManager->LoadAttackInformation(this->GetClass(),"DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'");
	dataManager->InitHitActors(this->GetClass(),m_HitActorsByMe);

	
	UDebugManager* debugManager = GetGameInstance()->GetSubsystem<UDebugManager>();
	check(debugManager != nullptr);
	
	debugManager->OnDebugMode.AddUObject(this, &AMainPlayer::printLog);
}

void AMainPlayer::OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation* AttackInformation, AActor* instigator, const FVector& causerLocation)
{
	if (m_bIsParrying)
	{
		ACharacterBase* attacker = Cast<ACharacterBase>(instigator);
		if (attacker != nullptr)
		{
			attacker->OnDamageStamina(1000.0f);
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_ParryingShake);
			UGameplayStatics::PlaySoundAtLocation(this, m_ParryingSound, this->GetActorLocation());

			if (m_ParryingHitParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					m_ParryingHitParticle,
					this->GetActorLocation(),
					FRotator::ZeroRotator,
					true // bAutoDestroy
				);
			}
		}
	}
	else
	{
		Super::OnDamage(damage, bIsCriticalAttack, AttackInformation, instigator, causerLocation);
	}
}

void AMainPlayer::PlayOnHitEffect(const FHitInformation& hitInformation)
{
	if (m_OnHitCameraShake != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_OnHitCameraShake);
	}
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const APlayerController* playerController = Cast<APlayerController>(this->GetController());
	check(playerController != nullptr);
	
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	check(subSystem != nullptr);
	
	subSystem->ClearAllMappings();
	
	subSystem->AddMappingContext(m_InputMappingConfigs["Default_OnGround"].inputMappingContext,0);
	m_CurActiveMappingContexts.Add("Default_OnGround", 0);
	
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(enhancedInputComponent != nullptr);
	
	UMainPlayerSkillComponent* skillComponent = Cast<UMainPlayerSkillComponent>(m_SkillComponent);
	check(skillComponent != nullptr);
	
	// Completed : 눌렀다 뗐을 때,   Triggered : 누르고 있을 때
	
	// UI
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Open_EnvironmentSettings"], ETriggerEvent::Triggered, Cast<AMainPlayerController>(GetController()), &AMainPlayerController::OpenEnvironmentSettingsState);

	// Move (Default_OnGround)

	APlayableCharacter* playableCharacter = Cast<APlayableCharacter>(this);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Move"], ETriggerEvent::Triggered, playableCharacter, &APlayableCharacter::Move);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Move"], ETriggerEvent::Completed, playableCharacter, &APlayableCharacter::InitArrowKeys);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Look"], ETriggerEvent::Triggered, playableCharacter, &APlayableCharacter::Look);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Run"], ETriggerEvent::Triggered, playableCharacter,&APlayableCharacter::Run);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["StopRun"], ETriggerEvent::Triggered, playableCharacter,&APlayableCharacter::StopRun);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Targeting"], ETriggerEvent::Triggered, playableCharacter,&APlayableCharacter::ToggleLockOnMode);

	// StrikeAttack (Default_OnGround)
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Started, skillComponent, &UMainPlayerSkillComponent::ActivateStrikeAttack);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Completed, skillComponent, &UMainPlayerSkillComponent::DeactivateStrikeAttack);
	
	// Default_OnGround
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["NormalAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::NormalAttack_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["UpperAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::UpperAttack_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["DashAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::DashAttack_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Dodge_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Dodge_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Charging_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Charging_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Guard_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Guard_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Execution_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Execution_OnGround);
	
	// Default_InAir
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_InAir"].inputActions["NormalAttack_InAir"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::NormalAttack_InAir);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_InAir"].inputActions["EarthStrike_InAir"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::EarthStrike_InAir);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Default_InAir"].inputActions["DashAttack_InAir"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::DashAttack_InAir);

	
	// Charging Skill
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Charging_OnGround"].inputActions["ChargingCancel_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::ChargingCancel_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Charging_OnGround"].inputActions["Charging_ComboDashAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround);
	enhancedInputComponent->BindAction(m_InputMappingConfigs["Charging_OnGround"].inputActions["FateSealed"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::FateSealed);
	
}

void AMainPlayer::initAssets()
{
	m_SkillComponent = CreateDefaultSubobject<UMainPlayerSkillComponent>(TEXT("SkillComponent"));
	check(m_SkillComponent != nullptr);
	
	// RootCapsuleComponent
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	
	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/MainPlayerAsset/Character/MainPlayer.MainPlayer'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	
	// rotation.y(pitch), rotation.z(yaw), rotation.x(roll)
	// location.x, location.y, location.z
	// scale.x, scale.y, scale.z
	

	// HitCollider
	m_HitCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollider"));
	check(m_HitCollider != nullptr);
	
	m_HitCollider->SetupAttachment(GetMesh(), FName(TEXT("spine_01")));
	m_HitCollider->SetCapsuleHalfHeight(60.0f);
	m_HitCollider->SetCapsuleRadius(60.0f);
	m_HitCollider->SetCollisionProfileName(TEXT("HitCollider_Player")); 
	m_HitCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_HitCollider->SetNotifyRigidBodyCollision(false);
	m_HitCollider->SetGenerateOverlapEvents(true);
	
	
	// SwordCollider
	FTransform collisionTransform = { {0.0f, 90.0f, -2.0f}, {0.62819f, 1.998782f, 97.919239f}, {0.625f, 0.75f, 1.0f} };

	m_SwordCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollider"));
	check(m_SwordCollider != nullptr);
	
	m_SwordCollider->SetupAttachment(GetMesh(), FName(TEXT("sword_bottom")));
	m_SwordCollider->SetWorldTransform(collisionTransform);
	m_SwordCollider->SetCapsuleHalfHeight(50.0f);
	m_SwordCollider->SetCapsuleRadius(18.0f);
	m_SwordCollider->SetCollisionProfileName(TEXT("AttackCollider_Player"));
	m_SwordCollider->SetGenerateOverlapEvents(true); // 블루프린트의 Generate Overlap Events에 대응되는 코드.
	m_SwordCollider->SetNotifyRigidBodyCollision(false);
	m_SwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 필요할때만 키기
	
	
	
	// ShieldCollider For Attack
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {2.6047f, 0.0f, 24.77f}, {1.0f, 2.0f, 1.1f} };

	m_ShieldForAttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldForAttackCollider"));
	check(m_ShieldForAttackCollider != nullptr);
	
	m_ShieldForAttackCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldForAttackCollider->SetWorldTransform(collisionTransform);
	m_ShieldForAttackCollider->SetCollisionProfileName(TEXT("AttackCollider_Player"));
	m_ShieldForAttackCollider->SetGenerateOverlapEvents(true);
	m_ShieldForAttackCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldForAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	// ShieldCollider For Defend
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {0.375f, 0.625f, 0.225f} };

	m_ShieldCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldForGuardCollider"));
	check(m_ShieldCollider != nullptr);
	
	m_ShieldCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldCollider->SetWorldTransform(collisionTransform);
	m_ShieldCollider->SetCollisionProfileName(TEXT("GuardCollider_Player"));
	m_ShieldCollider->SetGenerateOverlapEvents(true);
	m_ShieldCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	// ShieldBotoomCollider
	
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {49.24f, 0.0f, 1.31f}, {1.0f, 1.25f, 0.1f} };
	
	m_ShieldBottomCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldBottomCollider"));
	check(m_ShieldBottomCollider != nullptr);
	
	m_ShieldBottomCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldBottomCollider->SetWorldTransform(collisionTransform);
	m_ShieldBottomCollider->SetCollisionProfileName(TEXT("ShieldBottomCollider_Player"));
	m_ShieldBottomCollider->SetGenerateOverlapEvents(false);
	m_ShieldBottomCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldBottomCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_Colliders.Add(TEXT("HitCollider"),m_HitCollider);
	m_Colliders.Add(SwordColliderName,m_SwordCollider);
	m_Colliders.Add(ShieldForAttackColliderName,m_ShieldForAttackCollider);
	m_Colliders.Add(ShieldForGuardColliderName, m_ShieldCollider);
	m_Colliders.Add(TEXT("ShieldBottomCollider"),m_ShieldBottomCollider);
}

void AMainPlayer::printLog()
{
	const FVector location = GetActorLocation();
	const FVector velocity = GetVelocity();
	const FVector forwardVector = GetActorForwardVector();
	
	GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Green, FString::Printf(TEXT("Location : %f  %f  %f"), location.X, location.Y, location.Z));
	GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, FString::Printf(TEXT("Velocity : %f  %f  %f"), velocity.X, velocity.Y, velocity.Z));
	GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Green, FString::Printf(TEXT("Forward : %f  %f  %f"), forwardVector.X, forwardVector.Y, forwardVector.Z));
	GEngine->AddOnScreenDebugMessage(3, 0.1f, FColor::Green, FString::Printf(TEXT("Velocity Length(speed) : %f"), m_CurSpeed));
	GEngine->AddOnScreenDebugMessage(4, 0.1f, FColor::Green, FString::Printf(TEXT("is inputVertical : %d"), m_CurInputVertical));
	GEngine->AddOnScreenDebugMessage(5, 0.1f, FColor::Green, FString::Printf(TEXT("is inputHorizontal : %d"), m_CurInputHorizontal));
	                                   
	const FString movementMode = GetCharacterMovement()->GetMovementName();
	FString log = "MainPlyaerMovement Mode :: ";
	log += movementMode;
	GEngine->AddOnScreenDebugMessage(6, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log));
	
	FString crowdState = Utility::ConvertEnumToString(m_CrowdControlComponent->GetCrowdControlState());
	FString log2 = Tags[0].ToString() + " :: CrowdState :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(7, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log2));
	
	const FName priorityName = GetHighestPriorityInputMappingContext();
	FString log3 = Tags[0].ToString() + " :: HighestPriorityInputMappingContext :: " + priorityName.ToString();
	GEngine->AddOnScreenDebugMessage(8, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log3));
	
	const FString bIsSuperArmor = FString::FromInt(m_bIsSuperArmor);
	FString log4 = Tags[0].ToString() + " :: Is SuperArmor :: " + bIsSuperArmor;
	GEngine->AddOnScreenDebugMessage(9, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log4));

	const FString bIsGuarding = FString::FromInt(m_bIsParrying);
	FString log5 = Tags[0].ToString() + " :: Is Parrying :: " + bIsGuarding;
	GEngine->AddOnScreenDebugMessage(10, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log5));
	
	// const FString log6 = "Controller Yaw :: " + FString::SanitizeFloat(GetController()->GetControlRotation().Yaw);
	// GEngine->AddOnScreenDebugMessage(11, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log6));
	//
	// const FString log7 = "Camera Yaw :: " + FString::SanitizeFloat(m_TargetCamera->GetComponentRotation().Yaw);
	// GEngine->AddOnScreenDebugMessage(12, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log7));
	//
	// const FString log8 = "Cur Stamina :: " + FString::SanitizeFloat(m_StatComponent->GetCurStamina());
	// GEngine->AddOnScreenDebugMessage(13, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log8));

	const FString log9 = "Is LockOn?? :: " + FString::FromInt(m_bIsLockOnMode);
	GEngine->AddOnScreenDebugMessage(14, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log9));

	const FString log10 = "Cur HP :: " + FString::SanitizeFloat(m_StatComponent->GetCurHP());
	GEngine->AddOnScreenDebugMessage(15, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log10));

	
	const FString log11 = "Cur Stamina :: " + FString::SanitizeFloat(m_StatComponent->GetCurStamina());
	GEngine->AddOnScreenDebugMessage(16, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log11));
	
	
	GEngine->AddOnScreenDebugMessage(20, 0.1f, FColor::Green, FString::Printf(TEXT("==============================")));
}
