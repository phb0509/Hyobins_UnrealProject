// Fill out your copyright notice in the Description page of Project Settings.


//#include "MainPlayer.h"
#include "MainPlayer/MainPlayer.h"
#include <GameFramework/SpringArmComponent.h.>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include "Utility/EnumTypes.h" 


AMainPlayer::AMainPlayer() : 
	m_ArmLengthTo(0.0f),
	m_ArmRotationTo(FRotator::ZeroRotator),
	m_ArmLengthSpeed(0.0f),
	m_ArmRotationSpeed(0.0f),
	m_WalkSpeed(300.0f),
	m_RunSpeed(700.0f),
	m_CurSpeed(0.0f),
	m_bIsRun(false),
	m_CurState(EMainPlayerStates::Attack)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(100.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);

	initControlSetting();

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
	ConstructorHelpers::FClassFinder<UAnimInstance>
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
	
	// SpringArm�� ���̿� ȸ�� ���� ��ǥ ������ ������ ����ǵ��� ����.
	if (IsValid(m_SpringArm))
	{
		m_SpringArm->TargetArmLength = FMath::FInterpTo(m_SpringArm->TargetArmLength, m_ArmLengthTo, DeltaTime, m_ArmLengthSpeed);
		m_DebugInt = 10;
	}
	else
	{
		m_DebugInt = 20;
	}

	// �α� 
	printLog();
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
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AMainPlayer::OnBeginRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AMainPlayer::OnEndRun);
}

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMainPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void AMainPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AMainPlayer::InputHorizontal(float value)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), value);
}

void AMainPlayer::InputVertical(float value)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), value);
}

void AMainPlayer::OnBeginRun()
{
	m_bIsRun = true;
	GetCharacterMovement()->MaxWalkSpeed = m_RunSpeed;
}

void AMainPlayer::OnEndRun()
{
	m_bIsRun = false;
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
}

void AMainPlayer::initControlSetting()
{
	m_ArmLengthSpeed = 3.0f;
	m_ArmRotationSpeed = 10.0f;
	m_ArmLengthTo = 450.0f;

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	m_SpringArm->TargetArmLength = 600;

	if (IsValid(m_SpringArm))
	{
		m_DebugInt2 = 11111;
	}
	else
	{
		m_DebugInt2 = 000000;
	}
	
	m_TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	m_TargetCamera->SetupAttachment(m_SpringArm);

	// ���������� ȸ�� ���� ��Ʈ�� ȸ�� ���� �����ϰ� �����ش�.
	m_SpringArm->bUsePawnControlRotation = true;
	m_SpringArm->bInheritPitch = true;
	m_SpringArm->bInheritRoll = true;
	m_SpringArm->bInheritYaw = true;

	// true�� �� ���, ī�޶�� ĳ���ͻ��̿� ��ֹ��� ���� ���, �� ����� Ȱ��ȭ ���ش�.
	m_SpringArm->bDoCollisionTest = true;

	// true�� �� ���, ��Ʈ�ѷ��� ȸ���������� ĳ���͸� ȸ��������.
	bUseControllerRotationYaw = false;

	// true�� �� ���, ĳ���Ͱ� �����̷��� �������� ĳ���͸� �ڵ����� ȸ������ �ش�.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ��Ʈ�ѷ��� "���ϴ�" �������� ĳ���͸� ȸ���Ѵ�.
	// ��, ������+���� ������ "��Ȯ��" 45�� �������� ĳ���Ͱ� ȸ���ؼ� �̵��ϴ� ���̴�.
	// ������ ĳ������ ȸ�� ������ "���� ��������" ������ �ش�.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// ȸ���� �ε巴�� ����� �ֱ� ���� RotationRate �� �����Ѵ�.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;



}

void AMainPlayer::printLog()
{
	FVector location = GetActorLocation();
	FVector velocity = GetVelocity();
	float size = velocity.Size();

	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString::Printf(TEXT("location : %f  %f  %f"), location.X, location.Y, location.X));
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Green, FString::Printf(TEXT("velocity : %f  %f  %f"), velocity.X, velocity.Y, velocity.X));
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("velocity Length : %f"), size));
	GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Green, FString::Printf(TEXT("is Run? : %d"), m_bIsRun));
	GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Green, FString::Printf(TEXT("curState : %d"), GetFSMState()));
	GEngine->AddOnScreenDebugMessage(6, 3.f, FColor::Green, FString::Printf(TEXT("Tick springArm is Valid : %d"), m_DebugInt));
	GEngine->AddOnScreenDebugMessage(7, 3.f, FColor::Green, FString::Printf(TEXT("init springArm is Valid : %d"), m_DebugInt2));
}
