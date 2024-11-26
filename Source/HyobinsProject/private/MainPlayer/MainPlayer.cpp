// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayerController.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/Utility.h"
#include <GameFramework/SpringArmComponent.h.>
#include <Camera/CameraComponent.h>
#include "SubSystems/DataManager.h"
#include "MotionWarpingComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



const int32 AMainPlayer::m_DirectionIndex[3][3] =
	{{5,4,3},
	{6,0,2},
	{7,0,1}
	};

const FName AMainPlayer::SwordColliderName = "SwordCollider";
const FName AMainPlayer::ShieldForAttackColliderName = "ShieldForAttackCollider";
const FName AMainPlayer::ShieldForDefendColliderName = "ShieldForDefendCollider";

AMainPlayer::AMainPlayer() :
	m_MoveDeltaSecondsOffset(20000.0f),
	m_RotationDeltaSecondsOffset(50.0f),
	m_bIsPressedShift(false),
	m_CurInputHorizontal(0),
	m_CurInputVertical(0)
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

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_SkillComponent->InitializeComponent();
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
		subSystem->AddMappingContext(m_InputMappingContextInAir,1);
	}
}

void AMainPlayer::RemoveInputContextMappingInAir()
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (subSystem->HasMappingContext(m_InputMappingContextInAir))
	{
		subSystem->RemoveMappingContext(m_InputMappingContextInAir);
	}
}

void AMainPlayer::AddInputContextMappingOnCharging() const
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (!subSystem->HasMappingContext(m_InputMappingContextOnCharging))
	{
		subSystem->AddMappingContext(m_InputMappingContextOnCharging,1);
	}
}

void AMainPlayer::RemoveInputContextMappingOnCharging() const
{
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (subSystem->HasMappingContext(m_InputMappingContextOnCharging))
	{
		subSystem->RemoveMappingContext(m_InputMappingContextOnCharging);
	}
}

void AMainPlayer::RotateActorToKeyInputDirection() // WSAD 키입력방향으로 액터회전.
{
	FRotator actorRotation = GetActorRotation();
	const double degree = Utility::ConvertToDegree(m_CurInputVertical, m_CurInputHorizontal);
	actorRotation.Yaw = GetControlRotation().Yaw + degree;
	
	SetActorRotation(actorRotation);
}

void AMainPlayer::RotateActorToControllerYaw() // 액터의 z축회전값을 컨트롤러의 z축회전값으로 변경.
{
	FRotator actorRotation = GetActorRotation();
	actorRotation.Yaw =  GetControlRotation().Yaw;
	
	SetActorRotation(actorRotation);
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
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	
	// Completed : 눌렀다 뗐을 때,   Triggered : 누르고 있을 때
	
	// UI
	EIC->BindAction(m_InputActionsOnGround["Open_EnvironmentSettings"], ETriggerEvent::Triggered, Cast<AMainPlayerController>(GetController()),&AMainPlayerController::OpenEnvironmentSettingsState);

	// Move
	EIC->BindAction(m_InputActionsOnGround["Move"], ETriggerEvent::Triggered, this, &AMainPlayer::Move);
	EIC->BindAction(m_InputActionsOnGround["Move"], ETriggerEvent::Completed, this, &AMainPlayer::InitArrowKeys);
	EIC->BindAction(m_InputActionsOnGround["Look"], ETriggerEvent::Triggered, this, &AMainPlayer::Look);
	EIC->BindAction(m_InputActionsOnGround["Run"], ETriggerEvent::Triggered, this,&AMainPlayer::Run);
	EIC->BindAction(m_InputActionsOnGround["StopRun"], ETriggerEvent::Triggered, this,&AMainPlayer::StopRun);

	// Skill_OnGround
	EIC->BindAction(m_InputActionsOnGround["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Started, m_SkillComponent.Get(), &UMainPlayerSkillComponent::ActivateStrikeAttack);
	EIC->BindAction(m_InputActionsOnGround["SetModifierKeyInput_StrikeAttack"], ETriggerEvent::Completed, m_SkillComponent.Get(), &UMainPlayerSkillComponent::DeactivateStrikeAttack);
	
	EIC->BindAction(m_InputActionsOnGround["NormalAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::NormalAttack_OnGround);
	EIC->BindAction(m_InputActionsOnGround["UpperAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::UpperAttack_OnGround);
	EIC->BindAction(m_InputActionsOnGround["DashAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::DashAttack_OnGround);
	EIC->BindAction(m_InputActionsOnGround["Dodge_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::Dodge_OnGround);

	EIC->BindAction(m_InputActionsOnGround["Charging_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::Charging_OnGround);
	
	// Skill_InAir
	EIC->BindAction(m_InputActionsInAir["NormalAttack_InAir"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::NormalAttack_InAir);
	EIC->BindAction(m_InputActionsInAir["EarthStrike_InAir"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::EarthStrike_InAir);
	EIC->BindAction(m_InputActionsInAir["DashAttack_InAir"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::DashAttack_InAir);

	
	// Charging Skill
	EIC->BindAction(m_InputActionsOnCharging["Charging_ComboDashAttack_OnGround"], ETriggerEvent::Triggered, m_SkillComponent.Get(), &UMainPlayerSkillComponent::Charging_ComboDashAttack_OnGround);
	
}

FVector AMainPlayer::GetForwardVectorFromControllerYaw() const
{
	const FRotator rotator = {0.0f, GetControlRotation().Yaw, 0.0f};
	
	return rotator.Vector();
}

FVector AMainPlayer::GetRightVectorFromControllerYaw() const
{
	const FRotator controllerYawRotator(0.0f, GetControlRotation().Yaw, 0.0f); // 컨트롤러 Yaw를 기준으로한 임시값.
	const FVector rightVector = FRotationMatrix(controllerYawRotator).GetUnitAxis(EAxis::Y);  // Right 방향
	
	return rightVector;
}

FVector AMainPlayer::GetControllerKeyInputDirectionVector(const int32 keyInputDirection) const
{
	// keyInputDirection == 0 ~ 7까지의 8방향. 전방 ~ 좌상.
	const float controllerYaw = GetController()->GetControlRotation().Yaw + 45.0f * keyInputDirection;
	const FRotator rotation = {0.0f, controllerYaw, 0.0f};
	
	return rotation.Vector();
}

int32 AMainPlayer::GetLocalDirection(const FVector& otherDirectionVector) const
{
	const FVector localDirection = GetActorTransform().InverseTransformVector(otherDirectionVector);
	const float radian = FMath::Atan2(localDirection.Y, localDirection.X);

	constexpr float range = PI / 8; // 22.5도

	if (radian >= -range && radian < range) // 앞쪽
	{
		return 0;
	}

	if (radian >= range && radian < 3 * range) 
	{
		return 1;
	}
	
	if (radian >= 3 * range && radian < 5 *	range) // 오른쪽
	{
		return 2;
	}

	if (radian >= 5 * range && radian < 7 * range) 
	{
		return 3;
	}

	// if ((radian < -7 * range && radian >= -8 * range) ||
	// 	(radian >= 7 * range && radian <= 8 * range)) // 뒤쪽
	// {
	// 	return 4; 
	// }
	
	if (radian < -5 * range && radian >= -7 * range) 
	{
		return 5;
	}

	if (radian < -3 * range && radian >= -5 * range) // 왼쪽
	{
		return 6;
	}
	
	if (radian < -range && radian >= -3 * range) 
	{
		return 7;
	}
	
	return 4;
}

void AMainPlayer::initAssets()
{
	m_SkillComponent = CreateDefaultSubobject<UMainPlayerSkillComponent>(TEXT("SkillComponent"));
	m_MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	
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
	checkf(IsValid(mesh.Object), TEXT("Mesh isn't Valid"));

	// SpringArm
	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	m_SpringArm->TargetArmLength = 600;

	// 스프링암의 회전 값을 컨트롤 회전 값과 동일하게 맞춰준다.
	m_SpringArm->bUsePawnControlRotation = true;
	m_SpringArm->bInheritPitch = true;
	m_SpringArm->bInheritRoll = true;
	m_SpringArm->bInheritYaw = true;

	// true로 할 경우, 카메라와 캐릭터사이에 장애물이 있을 경우, 줌 기능을 활성화 해준다.
	m_SpringArm->bDoCollisionTest = false;

	// TargetCamera
	m_TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	m_TargetCamera->SetupAttachment(m_SpringArm);


	// rotation.y(pitch), rotation.z(yaw), rotation.x(roll)
	// location.x, location.y, location. z
	// scale.x,scale.y,scale.z

	// SwordCollider
	FTransform collisionTransform = { {0.0f, 90.0f, -2.0f}, {0.279196f, 1.998782f, 87.925328f}, {0.5f, 0.5f, 1.0f} };

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
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {1.0f, 1.25f, 0.35f} };

	m_ShieldForAttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldForAttackCollider"));
	m_ShieldForAttackCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldForAttackCollider->SetWorldTransform(collisionTransform);
	m_ShieldForAttackCollider->SetCollisionProfileName(TEXT("AttackCollider_Player"));
	m_ShieldForAttackCollider->SetGenerateOverlapEvents(true);
	m_ShieldForAttackCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldForAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	// ShieldCollider For Shield
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {1.0f, 1.25f, 0.1f} };

	m_ShieldForDefendCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldForDefendCollider"));
	m_ShieldForDefendCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldForDefendCollider->SetWorldTransform(collisionTransform);
	m_ShieldForDefendCollider->SetCollisionProfileName(TEXT("DefendCollider_Player"));
	m_ShieldForDefendCollider->SetGenerateOverlapEvents(true);
	m_ShieldForDefendCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldForDefendCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
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
	
	// 이외 CharacterMovement Detail값들

	// true로 할 경우, 컨트롤러의 회전방향으로 캐릭터를 회전시켜줌.
	bUseControllerRotationYaw = false;

	// true로 할 경우, 캐릭터가 움직이려는 방향으로 캐릭터를 자동으로 회전시켜 준다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// 컨트롤러가 "원하는" 방향으로 캐릭터를 회전한다.
	// 즉, 오른쪽+위를 누르면 "정확히" 45도 방향으로 캐릭터가 회전해서 이동하는 식이다.
	// 실제로 캐릭터의 회전 방향이 "딱딱 떨어지는" 느낌을 준다.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 회전을 부드럽게 만들어 주기 위해 RotationRate 를 조정한다. 값이 낮을수록 카메라 회전시 캐릭터가 한박자 느리게 회전.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
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
	FString log3 = "MainPlyaerMovement Mode :: ";
	log3 += movementMode;
	GEngine->AddOnScreenDebugMessage(6, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *log3));
	GEngine->AddOnScreenDebugMessage(7, 3.f, FColor::Green, FString::Printf(TEXT("==============================")));
}