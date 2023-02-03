// Fill out your copyright notice in the Description page of Project Settings.


//#include "MainPlayer.h"
#include "MainPlayer/MainPlayer.h"
#include <GameFramework/SpringArmComponent.h.>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>

#include "Utility/EnumTypes.h" 



AMainPlayer::AMainPlayer() :
	m_ArmLengthTo(450.0f),
	m_ArmRotationTo(10.0f),
	m_ArmLengthSpeed(3.0f),
	m_ArmRotationSpeed(0.0f),
	m_CurSpeed(0.0f),
	m_WalkSpeed(300.0f),
	m_RunSpeed(900.0f),
	m_MovdDeltaSecondsOffset(20000.0f),
	m_RotationDeltaSecondsOffset(50.0f),
	m_bIsIdle(true),
	m_bIsPressingShift(false),
	m_bIsCombated(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsInAir(false),
	m_bIsAttacking(false),
	m_bIsHit(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	initComponents();

	// �޽� �ε�.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		tempMesh(TEXT("SkeletalMesh'/Game/MainPlayerAsset/Character/MainPlayer.MainPlayer'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//�ִϸ��̼� ��� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MainPlayer_AnimInstance(TEXT("AnimBlueprint'/Game/Blueprints/ABP_MainPlayer.ABP_MainPlayer_C'"));

	if (MainPlayer_AnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MainPlayer_AnimInstance.Class);
	}

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
}


// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkIsValidComponants();
	updateState();
	m_SpringArm->TargetArmLength = FMath::FInterpTo(m_SpringArm->TargetArmLength, m_ArmLengthTo, DeltaTime, m_ArmLengthSpeed);

	// �α� 
	printLog();
}

void AMainPlayer::updateState()
{
	m_CurSpeed = GetVelocity().Size();
	m_bIsInAir = GetMovementComponent()->IsFalling();

	if (!m_bIsInAir)
	{
		if (m_CurSpeed < 0.1)
		{
			m_bIsIdle = true;
			m_bIsWalking = false;
			m_bIsRunning = false;
		}
		else if (m_CurSpeed > 0.1 && m_CurSpeed <= m_WalkSpeed)
		{
			m_bIsIdle = false;
			m_bIsWalking = true;
			m_bIsRunning = false;
		}
		else if (m_CurSpeed > m_WalkSpeed && m_CurSpeed <= m_RunSpeed)
		{
			m_bIsIdle = false;
			m_bIsWalking = false;
			m_bIsRunning = true;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Green, FString::Printf(TEXT("Is Jumping!!!!!")));
	}

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// AxisMappings
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainPlayer::Turn); // Mouse X 
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainPlayer::LookUp); // Mouse Y
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMainPlayer::InputHorizontal); // ��, �� (A,D) 
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AMainPlayer::InputVertical); // ��,�� (W,S)

	// ActionMappings
	PlayerInputComponent->BindAction(TEXT("LeftShift"), IE_Pressed, this, &AMainPlayer::TriggerPressedShift);
	PlayerInputComponent->BindAction(TEXT("LeftShift"), IE_Released, this, &AMainPlayer::TriggerReleasedShift);
	PlayerInputComponent->BindAction(TEXT("MoveWSAD"), IE_Pressed, this, &AMainPlayer::TriggerPressedMoveWSAD);
	PlayerInputComponent->BindAction(TEXT("MoveWSAD"), IE_Released, this, &AMainPlayer::TriggerReleasedMoveWSAD);
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Pressed, this, &AMainPlayer::TriggerPressedLeftMouseButton);
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Released, this, &AMainPlayer::TriggerReleasedLeftMouseButton);
}

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), value * GetWorld()->GetDeltaSeconds() * m_MovdDeltaSecondsOffset);
}

void AMainPlayer::InputVertical(float value)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), value * GetWorld()->GetDeltaSeconds() * m_MovdDeltaSecondsOffset);
}

void AMainPlayer::TriggerPressedShift()
{
	m_bIsPressingShift = true;
	GetCharacterMovement()->MaxWalkSpeed = m_RunSpeed;
}

void AMainPlayer::TriggerReleasedShift()
{
	m_bIsPressingShift = false;
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
}

void AMainPlayer::TriggerPressedMoveWSAD()
{

}

void AMainPlayer::TriggerReleasedMoveWSAD()
{

}

void AMainPlayer::TriggerPressedLeftMouseButton()
{
	m_bIsAttacking = true;
}

void AMainPlayer::TriggerReleasedLeftMouseButton()
{
	//m_bIsAttacking = false;
}

void AMainPlayer::initComponents()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(100.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);

	initSwordCollision();
	initSpringArm();
	initTargetCamera();

	// true�� �� ���, ��Ʈ�ѷ��� ȸ���������� ĳ���͸� ȸ��������.
	bUseControllerRotationYaw = false;

	// true�� �� ���, ĳ���Ͱ� �����̷��� �������� ĳ���͸� �ڵ����� ȸ������ �ش�.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ��Ʈ�ѷ��� "���ϴ�" �������� ĳ���͸� ȸ���Ѵ�.
	// ��, ������+���� ������ "��Ȯ��" 45�� �������� ĳ���Ͱ� ȸ���ؼ� �̵��ϴ� ���̴�.
	// ������ ĳ������ ȸ�� ������ "���� ��������" ������ �ش�.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// ȸ���� �ε巴�� ����� �ֱ� ���� RotationRate �� �����Ѵ�. ���� �������� ī�޶� ȸ���� ĳ���Ͱ� �ѹ��� ������ ȸ��.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
}

void AMainPlayer::initSwordCollision()
{
	FTransform collisionTransform = { {0.0f, 90.0f, -2.0f}, {0.279196f, 1.998782f, 87.925328f}, {0.5f, 0.5f, 1.0f} };

	// rotation.y(pitch), rotation.z(yaw), rotation.x(roll)
	// location.x, location.y, location. z
	// scale.x,scale.y,scale.z

	m_SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	m_SwordCollision->SetupAttachment(GetMesh(), FName(TEXT("sword_bottom")));
	m_SwordCollision->SetWorldTransform(collisionTransform);
	m_SwordCollision->SetCapsuleHalfHeight(50.0f);
	m_SwordCollision->SetCapsuleRadius(10.0f);
	m_SwordCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	
	m_SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainPlayer::initSpringArm()
{
	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	m_SpringArm->TargetArmLength = 600;

	// ���������� ȸ�� ���� ��Ʈ�� ȸ�� ���� �����ϰ� �����ش�.
	m_SpringArm->bUsePawnControlRotation = true;
	m_SpringArm->bInheritPitch = true;
	m_SpringArm->bInheritRoll = true;
	m_SpringArm->bInheritYaw = true;

	// true�� �� ���, ī�޶�� ĳ���ͻ��̿� ��ֹ��� ���� ���, �� ����� Ȱ��ȭ ���ش�.
	m_SpringArm->bDoCollisionTest = true;
}

void AMainPlayer::initTargetCamera()
{
	m_TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	m_TargetCamera->SetupAttachment(m_SpringArm);
}

void AMainPlayer::checkIsValidComponants()
{
	checkf(IsValid(m_SpringArm), TEXT("SpringArm is not Valid"));
	checkf(IsValid(m_TargetCamera), TEXT("TargetCamera is not Valid"));
}

void AMainPlayer::printLog()
{
	FVector location = GetActorLocation();
	FVector velocity = GetVelocity();

	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString::Printf(TEXT("location : %f  %f  %f"), location.X, location.Y, location.X));
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Green, FString::Printf(TEXT("velocity : %f  %f  %f"), velocity.X, velocity.Y, velocity.X));
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("velocity Length(speed) : %f"), m_CurSpeed));
}
