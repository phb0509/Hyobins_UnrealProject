// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerController.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Utility/Utility.h"
#include <GameFramework/SpringArmComponent.h.>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>

AMainPlayer::AMainPlayer() :
	m_ArmLengthTo(450.0f),
	m_ArmLengthSpeed(3.0f),
	m_MoveDeltaSecondsOffset(20000.0f),
	m_RotationDeltaSecondsOffset(50.0f),
	m_bIsCombated(true),
	m_bIsHit(false),
	m_bIsPressingShift(false),
	m_CurInputHorizontal(0),
	m_CurInputVertical(0),
	m_TempInputHorizontalForDodge(0),
	m_TempInputVerticalForDodge(0)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMainPlayerController::StaticClass();

	this->Tags.Add(FName("MainPlayer"));

	m_WalkSpeed = 300.0f;
	m_RunSpeed = 1300.0f;
	
	UE_LOG(LogTemp, Warning, TEXT("MainPlayer::Constructor"));

	m_SkillComponent = CreateDefaultSubobject<UMainPlayerSkillComponent>(TEXT("SkillComponent"));
	initAssets();
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("MainPlayer::BeginPlay"));
	
	SetActorLocation(FVector(0.0f, 0.0f, 200.0f));

	m_SwordCollider->OnComponentBeginOverlap.AddDynamic(m_SkillComponent, &UMainPlayerSkillComponent::OnCalled_Overlap_SwordCollision);
	m_ShieldForAttackCollider->OnComponentBeginOverlap.AddDynamic(m_SkillComponent, &UMainPlayerSkillComponent::OnCalled_Overlap_ShieldCollisionForAttack);
    m_ShieldForAttackCollider->OnComponentBeginOverlap.AddDynamic(m_SkillComponent, &UMainPlayerSkillComponent::OnCalled_Overlap_ShieldCollisionForDefend);
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
	m_SpringArm->TargetArmLength = FMath::FInterpTo(m_SpringArm->TargetArmLength, m_ArmLengthTo, DeltaTime, m_ArmLengthSpeed);

	printLog();
}

void AMainPlayer::Turn(float value)
{
	AddControllerYawInput(value * GetWorld()->GetDeltaSeconds() * m_RotationDeltaSecondsOffset);
}

void AMainPlayer::LookUp(float value)
{
	AddControllerPitchInput(value * GetWorld()->GetDeltaSeconds() * m_RotationDeltaSecondsOffset);
}

void AMainPlayer::InputHorizontal(float value)
{
	m_CurInputHorizontal = value;

	if (m_SkillComponent->GetSkillState() == EMainPlayerSkillStates::None) // 어떠한 스킬도 시전중 아니라면
	{
		const FVector worldDirection = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
		AddMovementInput(worldDirection, value * GetWorld()->GetDeltaSeconds() * m_MoveDeltaSecondsOffset);
	}
}

void AMainPlayer::InputVertical(float value)
{
	m_CurInputVertical = value;

	if (m_SkillComponent->GetSkillState() == EMainPlayerSkillStates::None)
	{
		const FVector worldDirection = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
		AddMovementInput(worldDirection, value * GetWorld()->GetDeltaSeconds() * m_MoveDeltaSecondsOffset);
	}
}

void AMainPlayer::TriggerPressedShift()
{
	m_bIsPressingShift = true;

	if (m_SkillComponent->GetSkillState() == EMainPlayerSkillStates::None)
	{
		GetCharacterMovement()->MaxWalkSpeed = m_RunSpeed;
	}
}

void AMainPlayer::TriggerReleasedShift()
{
	m_bIsPressingShift = false;
	
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
}

void AMainPlayer::TriggerPressedLeftMouseButton()
{
	if (GetMovementComponent()->IsMovingOnGround())
	{
		m_SkillComponent->NormalAttack_OnGround();
	}
}

void AMainPlayer::TriggerReleasedLeftMouseButton()
{
}

void AMainPlayer::TriggerPressedSpaceBar() 
{
	if (GetMovementComponent()->IsMovingOnGround())
	{
		m_SkillComponent->Dodge_OnGround();
	}
}

void AMainPlayer::TriggerPressedQ()
{
	if (GetMovementComponent()->IsMovingOnGround())
	{
		m_SkillComponent->UpperAttack_OnGround();
	}
}

void AMainPlayer::TriggerPressedLeftCtrl()
{

}

void AMainPlayer::TriggerReleasedLeftCtrl()
{

}

void AMainPlayer::ActivateSwordCollider()
{
	m_SwordCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainPlayer::DeactivateSwordCollider()
{
	m_SwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainPlayer::ActivateShieldForAttackCollider()
{
	m_ShieldForAttackCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainPlayer::DeactivateShieldForAttackCollider()
{
	m_ShieldForAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainPlayer::ActivateShieldForDefendCollider()
{
	m_ShieldForDefendCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainPlayer::DeactivateShieldForDefendCollider()
{
	m_ShieldForDefendCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainPlayer::initAssets()
{
	// RootCapsuleComponent
	checkf(GetCapsuleComponent() != nullptr, TEXT("CapsuleComponent isn't Valid"));
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
	m_SwordCollider->SetCapsuleRadius(10.0f);
	m_SwordCollider->SetCollisionProfileName(TEXT("AttackCollider"));
	m_SwordCollider->SetGenerateOverlapEvents(true); // 블루프린트의 Generate Overlap Events에 대응되는 코드.
	m_SwordCollider->SetNotifyRigidBodyCollision(false);
	m_SwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 필요할때만 키기
	
	
	// ShieldCollider For Attack
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {1.0f, 1.25f, 0.35f} };

	m_ShieldForAttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldColliderForAttack"));
	m_ShieldForAttackCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldForAttackCollider->SetWorldTransform(collisionTransform);
	m_ShieldForAttackCollider->SetCollisionProfileName(TEXT("AttackCollider"));
	m_ShieldForAttackCollider->SetGenerateOverlapEvents(true);
	m_ShieldForAttackCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldForAttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	// ShieldCollider For Shield
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {1.0f, 1.25f, 0.1f} };

	m_ShieldForDefendCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldColliderForShield"));
	m_ShieldForDefendCollider->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldForDefendCollider->SetWorldTransform(collisionTransform);
	m_ShieldForDefendCollider->SetCollisionProfileName(TEXT("HitCollider"));
	m_ShieldForDefendCollider->SetGenerateOverlapEvents(true);
	m_ShieldForDefendCollider->SetNotifyRigidBodyCollision(false);
	m_ShieldForDefendCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
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

void AMainPlayer::updateState()
{
	m_CurSpeed = GetVelocity().Size();

	if (GetCharacterMovement()->IsFalling())
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString::Printf(TEXT("Is Jumping!!!!!")));
	}
}

void AMainPlayer::printLog() const
{
	const FVector location = GetActorLocation();
	const FVector velocity = GetVelocity();
	const FVector forwardVector = GetActorForwardVector();

	FString curSkillState = Utility::ConvertEnumToString(m_SkillComponent->GetSkillState());
	
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Green, FString::Printf(TEXT("Location : %f  %f  %f"), location.X, location.Y, location.Z));
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("Velocity : %f  %f  %f"), velocity.X, velocity.Y, velocity.Z));
	GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Green, FString::Printf(TEXT("Forward : %f  %f  %f"), forwardVector.X, forwardVector.Y, forwardVector.Z));
	GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Green, FString::Printf(TEXT("Velocity Length(speed) : %f"), m_CurSpeed));
	//GEngine->AddOnScreenDebugMessage(6, 3.f, FColor::Green, FString::Printf(TEXT("CurCombo : %d"), m_CurNormalAttackCombo));
	GEngine->AddOnScreenDebugMessage(9, 3.f, FColor::Green, FString::Printf(TEXT("is inputVertical : %d"), m_CurInputVertical));
	GEngine->AddOnScreenDebugMessage(10, 3.f, FColor::Green, FString::Printf(TEXT("is inputHorizontal : %d"), m_CurInputHorizontal));
	GEngine->AddOnScreenDebugMessage(11, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *curSkillState));
}

void AMainPlayer::RotateActorToKeyInputDirection() // WSAD 키입력방향으로 액터회전.
{
	const FRotator controllerRotation = GetControlRotation();
	const FRotator actorRotation = GetActorRotation();
	const double degree = Utility::ConvertToDegree(m_CurInputVertical, m_CurInputHorizontal);
	const FRotator resultRotator = { actorRotation.Pitch, controllerRotation.Yaw + degree, actorRotation.Roll };
	
	SetActorRotation(resultRotator);
}

void AMainPlayer::RotateActorToControllerYaw()
{
	const FRotator controllerRotation = GetControlRotation();
	const FRotator actorRotation = GetActorRotation();
	const FRotator resultRotator = { actorRotation.Pitch, controllerRotation.Yaw, actorRotation.Roll };

	SetActorRotation(resultRotator);
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// AxisMappings
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainPlayer::Turn); // Mouse X 
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainPlayer::LookUp); // Mouse Y
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMainPlayer::InputHorizontal); // 좌, 우 (A,D) 
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AMainPlayer::InputVertical); // 전,후 (W,S)

	// ActionMappings
	PlayerInputComponent->BindAction(TEXT("LeftShift"), IE_Pressed, this, &AMainPlayer::TriggerPressedShift);
	PlayerInputComponent->BindAction(TEXT("LeftShift"), IE_Released, this, &AMainPlayer::TriggerReleasedShift);
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Pressed, this, &AMainPlayer::TriggerPressedLeftMouseButton);
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Released, this, &AMainPlayer::TriggerReleasedLeftMouseButton);
	PlayerInputComponent->BindAction(TEXT("SpaceBar"), IE_Pressed, this, &AMainPlayer::TriggerPressedSpaceBar);
	PlayerInputComponent->BindAction(TEXT("LeftCtrl"), IE_Pressed, this, &AMainPlayer::TriggerPressedLeftCtrl);
	PlayerInputComponent->BindAction(TEXT("Q"), IE_Pressed, this, &AMainPlayer::TriggerPressedQ);
}