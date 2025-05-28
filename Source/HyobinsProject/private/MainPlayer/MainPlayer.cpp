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
#include "SubSystems/DebugManager.h"


const FName AMainPlayer::SwordColliderName = "SwordCollider";
const FName AMainPlayer::ShieldForAttackColliderName = "ShieldForAttackCollider";
const FName AMainPlayer::ShieldForGuardColliderName = "ShieldForGuardCollider";

AMainPlayer::AMainPlayer() :
	m_bIsGuarding(false)
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
	
	SetActorLocation(FVector(0.0f, 0.0f, 40.0f));
	
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	dataManager->LoadAttackInformation(this->GetClass(),"DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'");
	dataManager->InitHitActors(this->GetClass(),m_HitActorsByMe);

	m_ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UDebugManager* debugManager = GetGameInstance()->GetSubsystem<UDebugManager>();
	if (debugManager != nullptr)
	{
		debugManager->OnDebugMode.AddUObject(this, &AMainPlayer::printLog);
	}
}

void AMainPlayer::Attack(const FName& attackName, AActor* target, const FVector& causerLocation)
{
	Super::Attack(attackName, target, causerLocation);
	
	playAttackEffect();
}

void AMainPlayer::OnDamage(const float damage, const bool bIsCriticalAttack, const FAttackInformation* AttackInformation, AActor* instigator, const FVector& causerLocation)
{
	Super::OnDamage(damage, bIsCriticalAttack, AttackInformation, instigator, causerLocation);
	// if (canGuard(instigator))
	// {
	// 	m_AnimInstanceBase->PlayMontage(TEXT("GuardHit_OnGround"));
	// 	UE_LOG(LogTemp, Warning, TEXT("AMainPlayer :: GuardHit"));
	// }
	// else
	// {
	// 	DisableGuard(); // 가드 강제해제.
	// 	Super::OnDamage(damage, bIsCriticalAttack, AttackInformation, instigator, causerLocation);
	// 	
	// 	UE_LOG(LogTemp, Warning, TEXT("AMainPlayer :: CCHit"));
	// }
}

void AMainPlayer::PlayOnHitEffect(const FHitInformation& hitInformation)
{
	if (m_OnHitCameraShake != nullptr)
    {
    	this->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_OnHitCameraShake);
    }
}

void AMainPlayer::playAttackEffect()
{
	// 카메라흔들림 적용
	UAnimMontage* curMontage = m_AnimInstanceBase->GetCurrentActiveMontage();
	m_AnimInstanceBase->Montage_SetPlayRate(curMontage,0.1f);
	
	FTimerHandle timer;
	GetWorldTimerManager().SetTimer
	( 
		timer,
		[=]()
		{
			m_AnimInstanceBase->Montage_SetPlayRate(curMontage,1.0f);
		},
		m_GameSpeedDelay,
		false
	);
	
	if (m_AttackCameraShake != nullptr)
	{
		this->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_AttackCameraShake);
	}
}

bool AMainPlayer::canGuard(const AActor* instigator) const
{
	return m_bIsGuarding && Utility::GetHitDirection(instigator->GetActorLocation(), this) == 0;
}

void AMainPlayer::EnableGuard()
{
	this->SetIsGuarding(true);
	this->GetStatComponent()->AddAdditionalDefenseFromGuard();
	this->GetCollider(TEXT("ShieldForGuardCollider"))->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AMainPlayer::DisableGuard()
{
	this->SetIsGuarding(false);
	this->SetIsSuperArmor(false, false);
	this->GetStatComponent()->RemoveAdditionalDefenseFromGuard();
	m_ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	subSystem->ClearAllMappings();
	
	subSystem->AddMappingContext(m_InputMappingConfigs["Default_OnGround"].inputMappingContext,0);
	m_CurActiveMappingContexts.Add("Default_OnGround", 0);
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	UMainPlayerSkillComponent* skillComponent = Cast<UMainPlayerSkillComponent>(m_SkillComponent);
	
	// Completed : 눌렀다 뗐을 때,   Triggered : 누르고 있을 때
	
	// UI
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Open_EnvironmentSettings"], ETriggerEvent::Triggered, Cast<AMainPlayerController>(GetController()), &AMainPlayerController::OpenEnvironmentSettingsState);

	// Move (Default_OnGround)

	APlayableCharacter* playableCharacter = Cast<APlayableCharacter>(this);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Move"], ETriggerEvent::Triggered, playableCharacter, &APlayableCharacter::Move);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Move"], ETriggerEvent::Completed, playableCharacter, &APlayableCharacter::InitArrowKeys);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Look"], ETriggerEvent::Triggered, playableCharacter, &APlayableCharacter::Look);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Run"], ETriggerEvent::Triggered, playableCharacter,&APlayableCharacter::Run);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["StopRun"], ETriggerEvent::Triggered, playableCharacter,&APlayableCharacter::StopRun);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Targeting"], ETriggerEvent::Triggered, playableCharacter,&APlayableCharacter::ToggleTargetMode);

	// StrikeAttack (Default_OnGround)
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Started, skillComponent, &UMainPlayerSkillComponent::ActivateStrikeAttack);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Completed, skillComponent, &UMainPlayerSkillComponent::DeactivateStrikeAttack);
	
	// Default_OnGround
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["NormalAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::NormalAttack_OnGround);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["UpperAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::UpperAttack_OnGround);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["DashAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::DashAttack_OnGround);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Dodge_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Dodge_OnGround);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Charging_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Charging_OnGround);
	EIC->BindAction(m_InputMappingConfigs["Default_OnGround"].inputActions["Guard_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Guard_OnGround);

	
	// Default_InAir
	EIC->BindAction(m_InputMappingConfigs["Default_InAir"].inputActions["NormalAttack_InAir"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::NormalAttack_InAir);
	EIC->BindAction(m_InputMappingConfigs["Default_InAir"].inputActions["EarthStrike_InAir"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::EarthStrike_InAir);
	EIC->BindAction(m_InputMappingConfigs["Default_InAir"].inputActions["DashAttack_InAir"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::DashAttack_InAir);

	
	// Charging Skill
	EIC->BindAction(m_InputMappingConfigs["Charging_OnGround"].inputActions["ChargingCancel_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::ChargingCancel_OnGround);
	EIC->BindAction(m_InputMappingConfigs["Charging_OnGround"].inputActions["Charging_ComboDashAttack_OnGround"], ETriggerEvent::Triggered, skillComponent, &UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround);
	
}

void AMainPlayer::initAssets()
{
	m_SkillComponent = CreateDefaultSubobject<UMainPlayerSkillComponent>(TEXT("SkillComponent"));
	
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
	m_ShieldForAttackCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldForAttackCollider->SetWorldTransform(collisionTransform);
	m_ShieldForAttackCollider->SetCollisionProfileName(TEXT("AttackCollider_Player"));
	m_ShieldForAttackCollider->SetGenerateOverlapEvents(true);
	m_ShieldForAttackCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldForAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	// ShieldCollider For Defend
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {0.375f, 0.625f, 0.225f} };

	m_ShieldCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldForGuardCollider"));
	m_ShieldCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldCollider->SetWorldTransform(collisionTransform);
	m_ShieldCollider->SetCollisionProfileName(TEXT("GuardCollider_Player"));
	m_ShieldCollider->SetGenerateOverlapEvents(true);
	m_ShieldCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//m_ShieldCollider->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::AMainPlayer::OnGuardOverlapBegin);

	// ShieldBotoomCollider
	
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {49.24f, 0.0f, 1.31f}, {1.0f, 1.25f, 0.1f} };
	
	m_ShieldBottomCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldBottomCollider"));
	m_ShieldBottomCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldBottomCollider->SetWorldTransform(collisionTransform);
	m_ShieldBottomCollider->SetCollisionProfileName(TEXT("ShieldBottomCollider_Player"));
	m_ShieldBottomCollider->SetGenerateOverlapEvents(false);
	m_ShieldBottomCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldBottomCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	m_Colliders.Add(SwordColliderName,m_SwordCollider);
	m_Colliders.Add(ShieldForAttackColliderName,m_ShieldForAttackCollider);
	m_Colliders.Add(ShieldForGuardColliderName, m_ShieldCollider);
	m_Colliders.Add(TEXT("ShieldBottomCollider"),m_ShieldBottomCollider);
	m_Colliders.Add(HitColliderName,m_ShieldBottomCollider);
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
	
	FString crowdState = Utility::ConvertEnumToString(m_CurCrowdControlState);
	FString log2 = Tags[0].ToString() + " :: CrowdState :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(7, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log2));
	
	const FName priorityName = GetHighestPriorityInputMappingContext();
	FString log3 = Tags[0].ToString() + " :: HighestPriorityInputMappingContext :: " + priorityName.ToString();
	GEngine->AddOnScreenDebugMessage(8, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log3));
	
	const FString bIsSuperArmor = FString::FromInt(m_bIsSuperArmor);
	FString log4 = Tags[0].ToString() + " :: Is SuperArmor :: " + bIsSuperArmor;
	GEngine->AddOnScreenDebugMessage(9, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log4));

	const FString bIsGuarding = FString::FromInt(m_bIsGuarding);
	FString log5 = Tags[0].ToString() + " :: Is Guarding :: " + bIsGuarding;
	GEngine->AddOnScreenDebugMessage(10, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log5));
	
	const FString log6 = "Controller Yaw :: " + FString::SanitizeFloat(GetController()->GetControlRotation().Yaw);
	GEngine->AddOnScreenDebugMessage(11, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log6));

	const FString log7 = "Camera Yaw :: " + FString::SanitizeFloat(m_TargetCamera->GetComponentRotation().Yaw);
	GEngine->AddOnScreenDebugMessage(12, 0.1f, FColor::Green, FString::Printf(TEXT("%s"), *log7));
	
	
	GEngine->AddOnScreenDebugMessage(10, 0.1f, FColor::Green, FString::Printf(TEXT("==============================")));
}
