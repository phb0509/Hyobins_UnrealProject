// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayerController.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/Utility.h"
#include "SubSystems/DataManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"


const FName AMainPlayer::SwordColliderName = "SwordCollider";
const FName AMainPlayer::ShieldForAttackColliderName = "ShieldForAttackCollider";
const FName AMainPlayer::ShieldForDefendColliderName = "ShieldForDefendCollider";

AMainPlayer::AMainPlayer() :
	m_bIsPressedShift(false)
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
	
	SetActorLocation(FVector(0.0f, 0.0f, 100.0f));
	
	UDataManager* dataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	dataManager->LoadAttackInformation(this->GetClass(),"DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'");
	dataManager->InitHitActors(this->GetClass(),m_HitActorsByMe);

	UMainPlayerAnim* animInstance = Cast<UMainPlayerAnim>(GetMesh()->GetAnimInstance());
	animInstance->OnEnteredState_Falling.AddDynamic(this, &AMainPlayer::AddInputContextMappingInAir);
	animInstance->OnEnteredState_MoveOnGround.AddDynamic(this, &AMainPlayer::RemoveInputContextMappingInAir);
}

void AMainPlayer::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	
	printLog();
}

void AMainPlayer::Run() const
{
	GetCharacterMovement()->MaxWalkSpeed = m_RunSpeed;
}

void AMainPlayer::StopRun() const
{
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
}

void AMainPlayer::AddInputContextMappingInAir()
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (!subSystem->HasMappingContext(m_InputMappingContextInAir))
	{
		const int8 priority = m_CurActiveMappingContexts.Num();
		m_CurActiveMappingContexts.Add("InAir", priority);
		subSystem->AddMappingContext(m_InputMappingContextInAir,priority);

		OnChangeInputMappingContext.Broadcast();
	}
}

void AMainPlayer::RemoveInputContextMappingInAir()
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (subSystem->HasMappingContext(m_InputMappingContextInAir))
	{
		subSystem->RemoveMappingContext(m_InputMappingContextInAir);
		m_CurActiveMappingContexts.Remove("InAir");

		OnChangeInputMappingContext.Broadcast();
	}
}

void AMainPlayer::AddInputContextMappingOnCharging()
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (!subSystem->HasMappingContext(m_InputMappingContextOnCharging))
	{
		const int8 priority = m_CurActiveMappingContexts.Num();
		m_CurActiveMappingContexts.Add("ChargingOnGround", priority);
		subSystem->AddMappingContext(m_InputMappingContextOnCharging,priority);

		OnChangeInputMappingContext.Broadcast();
	}
}

void AMainPlayer::RemoveInputContextMappingOnCharging()
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	
	if (subSystem->HasMappingContext(m_InputMappingContextOnCharging))
	{
		subSystem->RemoveMappingContext(m_InputMappingContextOnCharging);
		m_CurActiveMappingContexts.Remove("ChargingOnGround");

		OnChangeInputMappingContext.Broadcast();
	}
}


void AMainPlayer::Move(const FInputActionValue& value)
{
	const FVector2d movementVector = value.Get<FVector2D>();
	
	const FRotator rotation = GetControlRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);

	// ForwardVector
	const FVector forwardDireciton = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

	// RightVector
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	// Add Movement
	this->AddMovementInput(forwardDireciton, movementVector.X);
	this->AddMovementInput(rightDirection, movementVector.Y);

	m_CurInputHorizontal = movementVector.Y;
	m_CurInputVertical = movementVector.X;
}

void AMainPlayer::Look(const FInputActionValue& value)
{
	const FVector2d axisVector = value.Get<FVector2d>();

	this->AddControllerYawInput(axisVector.X);
	this->AddControllerPitchInput(axisVector.Y);
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	subSystem->ClearAllMappings();
	subSystem->AddMappingContext(m_InputMappingContextOnGround,0);
	m_CurActiveMappingContexts.Add("DefaultOnGround", 0);
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	
	// Completed : 눌렀다 뗐을 때,   Triggered : 누르고 있을 때
	
	// UI
	EIC->BindAction(m_InputActionsDefaultOnGround["Open_EnvironmentSettings"], ETriggerEvent::Triggered, Cast<AMainPlayerController>(GetController()),&AMainPlayerController::OpenEnvironmentSettingsState);

	// Move
	EIC->BindAction(m_InputActionsDefaultOnGround["Move"], ETriggerEvent::Triggered, this, &AMainPlayer::Move);
	EIC->BindAction(m_InputActionsDefaultOnGround["Move"], ETriggerEvent::Completed, this, &AMainPlayer::InitArrowKeys);
	EIC->BindAction(m_InputActionsDefaultOnGround["Look"], ETriggerEvent::Triggered, this, &AMainPlayer::Look);
	EIC->BindAction(m_InputActionsDefaultOnGround["Run"], ETriggerEvent::Triggered, this,&AMainPlayer::Run);
	EIC->BindAction(m_InputActionsDefaultOnGround["StopRun"], ETriggerEvent::Triggered, this,&AMainPlayer::StopRun);

	// Skill_OnGround
	EIC->BindAction(m_InputActionsDefaultOnGround["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Started, m_SkillComponent.Get(), &UMainPlayerSkillComponent::ActivateStrikeAttack);
	EIC->BindAction(m_InputActionsDefaultOnGround["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Completed, m_SkillComponent.Get(), &UMainPlayerSkillComponent::DeactivateStrikeAttack);
	
	EIC->BindAction(m_InputActionsDefaultOnGround["NormalAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::NormalAttack_OnGround);
	EIC->BindAction(m_InputActionsDefaultOnGround["UpperAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::UpperAttack_OnGround);
	EIC->BindAction(m_InputActionsDefaultOnGround["DashAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::DashAttack_OnGround);
	EIC->BindAction(m_InputActionsDefaultOnGround["Dodge_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::Dodge_OnGround);

	EIC->BindAction(m_InputActionsDefaultOnGround["Charging_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::Charging_OnGround);
	
	// Skill_InAir
	EIC->BindAction(m_InputActionsInAir["NormalAttack_InAir"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::NormalAttack_InAir);
	EIC->BindAction(m_InputActionsInAir["EarthStrike_InAir"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::EarthStrike_InAir);
	EIC->BindAction(m_InputActionsInAir["DashAttack_InAir"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::DashAttack_InAir);

	
	// Charging Skill
	EIC->BindAction(m_InputActionsOnCharging["Charging_ComboDashAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround);
	
}

void AMainPlayer::Attack(const FName& attackName, TWeakObjectPtr<AActor> target)
{
	Super::Attack(attackName, target);
	
	playAttackEffect();
}

void AMainPlayer::PlayOnHitEffect(const FHitInformation& hitInformation)
{
	if (m_OnHitCameraShake != nullptr)
    {
    	this->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(m_OnHitCameraShake);
    }
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
	// location.x, location.y, location. z
	// scale.x,scale.y,scale.z
	

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
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {1.375f, 1.625f, 0.225f} };

	m_ShieldForDefendCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldForDefendCollider"));
	m_ShieldForDefendCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldForDefendCollider->SetWorldTransform(collisionTransform);
	m_ShieldForDefendCollider->SetCollisionProfileName(TEXT("DefendCollider_Player"));
	m_ShieldForDefendCollider->SetGenerateOverlapEvents(true);
	m_ShieldForDefendCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldForDefendCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);


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
	m_Colliders.Add(ShieldForDefendColliderName,m_ShieldForDefendCollider);
	m_Colliders.Add(TEXT("ShieldBottomCollider"),m_ShieldBottomCollider);
	m_Colliders.Add(HitColliderName,m_ShieldBottomCollider);
}

void AMainPlayer::printLog() const
{
	const FVector location = GetActorLocation();
	const FVector velocity = GetVelocity();
	const FVector forwardVector = GetActorForwardVector();
	
	GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Green, FString::Printf(TEXT("Location : %f  %f  %f"), location.X, location.Y, location.Z));
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString::Printf(TEXT("Velocity : %f  %f  %f"), velocity.X, velocity.Y, velocity.Z));
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Green, FString::Printf(TEXT("Forward : %f  %f  %f"), forwardVector.X, forwardVector.Y, forwardVector.Z));
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("Velocity Length(speed) : %f"), m_CurSpeed));
	GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Green, FString::Printf(TEXT("is inputVertical : %d"), m_CurInputVertical));
	GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Green, FString::Printf(TEXT("is inputHorizontal : %d"), m_CurInputHorizontal));
	
	const FString movementMode = GetCharacterMovement()->GetMovementName();
	FString log = "MainPlyaerMovement Mode :: ";
	log += movementMode;
	GEngine->AddOnScreenDebugMessage(6, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log));
	
	FString crowdState = Utility::ConvertEnumToString(m_CurCrowdControlState);
	FString log2 = Tags[0].ToString() + " :: CrowdState :: " + crowdState;
	GEngine->AddOnScreenDebugMessage(7, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log2));
	
	const FName priorityName = GetHighestPriorityInputMappingContext();
	FString log3 = Tags[0].ToString() + " :: HighestPriorityInputMappingContext :: " + priorityName.ToString();
	GEngine->AddOnScreenDebugMessage(8, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log3));
	
	const FString bIsSuperArmor = FString::FromInt(m_bIsSuperArmor);
	FString log4 = Tags[0].ToString() + " :: Is SuperArmor :: " + bIsSuperArmor;
	GEngine->AddOnScreenDebugMessage(9, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log4));
	
	GEngine->AddOnScreenDebugMessage(10, 3.f, FColor::Green, FString::Printf(TEXT("==============================")));
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
