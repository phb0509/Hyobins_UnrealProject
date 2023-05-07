// Fill out your copyright notice in the Description page of Project Settings.


//#include "MainPlayer.h"
#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayerController.h"
#include <GameFramework/SpringArmComponent.h.>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include "Utility/EnumTypes.h" 
#include "DrawDebugHelpers.h"


AMainPlayer::AMainPlayer() :
	m_ArmLengthTo(450.0f),
	m_ArmRotationTo(10.0f),
	m_ArmLengthSpeed(3.0f),
	m_ArmRotationSpeed(0.0f),
	m_MovdDeltaSecondsOffset(20000.0f),
	m_RotationDeltaSecondsOffset(50.0f),
	m_bIsPressingShift(false),
	m_bIsCombated(true),
	m_bIsHit(false),
	m_bCanNextCombo(false),
	m_bIsInputOnNextCombo(false),
	m_CurNormalAttackCombo(0),
	m_NormalAttackMaxCombo(4),
	m_NormalAttackRange(50.0f),
	m_NormalAttackRadius(70.0f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMainPlayerController::StaticClass();

	this->Tags.Add(FName("MainPlayer"));

	m_WalkSpeed = 300.0f;
	m_RunSpeed = 1300.0f;

	initAssets();
	initAttackInformations("DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'");
	updateNormalAttackStateOnEnd();
}

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_AnimInstance = Cast<UMainPlayerAnim>(GetMesh()->GetAnimInstance());
	m_AnimInstance->OnMontageEnded.AddDynamic(this, &AMainPlayer::OnNormalAttackMontageEnded);
	m_AnimInstance->OnNextNormalAttackCheck.AddUObject(this, &AMainPlayer::OnCalledCheckNextAttackNotify); // ��Ƽ������ BroadCast ���޹����� ���ε��� �Լ� ȣ��.
	m_AnimInstance->OnNormalAttackHitCheck.AddUObject(this, &AMainPlayer::CheckNormalAttackCollision);
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(-200.0f, 0.0f, 100.0f));
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
	m_SpringArm->TargetArmLength = FMath::FInterpTo(m_SpringArm->TargetArmLength, m_ArmLengthTo, DeltaTime, m_ArmLengthSpeed);

	printLog();
}

void AMainPlayer::normalComboAttack()
{
	if (m_bIsAttacking) 
	{
		if (m_bCanNextCombo)
		{
			m_bIsInputOnNextCombo = true;
		}
	}
	else
	{
		updateNormalAttackStateOnStart();
		m_AnimInstance->PlayNormalAttackMontage();
		m_AnimInstance->JumpToNormalAttackMontageSection(m_CurNormalAttackCombo); // 0(������)���� 1�� ����
		m_bIsAttacking = true;
	}
}

void AMainPlayer::OnNormalAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) // �⺻���ݸ�Ÿ�ְ� ������ ��� �Ǿ��ų�, ���� ���� Ű�Է��� ���̻� ���ų�
{
	m_bIsAttacking = false;
	updateNormalAttackStateOnEnd();
}

void AMainPlayer::OnCalledCheckNextAttackNotify()
{
	m_bCanNextCombo = false;

	if (m_bIsInputOnNextCombo) // ������ Ÿ�ֿ̹� Ű�Է� �Ǹ�
	{
		updateNormalAttackStateOnStart();
		m_AnimInstance->JumpToNormalAttackMontageSection(m_CurNormalAttackCombo);
	}
}

void AMainPlayer::updateNormalAttackStateOnStart() // �� ������ �⺻����(���Ӱ����̴ϱ�) ���� ��, ���°� ������Ʈ.
{
	m_bCanNextCombo = true;
	m_bIsInputOnNextCombo = false;
	m_CurNormalAttackCombo = FMath::Clamp<int32>(m_CurNormalAttackCombo + 1, 1, m_NormalAttackMaxCombo);
}

void AMainPlayer::updateNormalAttackStateOnEnd() // �⺻������ �ƿ� ���� ��(������ ��� or Ű�Է�x), ���°� ������Ʈ.
{
	m_bIsInputOnNextCombo = false;
	m_bCanNextCombo = false;
	m_CurNormalAttackCombo = 0;
}

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
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Pressed, this, &AMainPlayer::TriggerPressedLeftMouseButton);
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Released, this, &AMainPlayer::TriggerReleasedLeftMouseButton);
}

void AMainPlayer::Jump()
{
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
	if (!m_bIsAttacking)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), value * GetWorld()->GetDeltaSeconds() * m_MovdDeltaSecondsOffset);
	}
}

void AMainPlayer::InputVertical(float value)
{
	if (!m_bIsAttacking)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), value * GetWorld()->GetDeltaSeconds() * m_MovdDeltaSecondsOffset);
	}
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

void AMainPlayer::TriggerPressedLeftMouseButton()
{
	normalComboAttack();
}

void AMainPlayer::TriggerReleasedLeftMouseButton()
{
	
}

void AMainPlayer::CheckNormalAttackCollision()
{
	TArray<FHitResult> detectedObjects;
	FCollisionQueryParams Params(NAME_None, false, this); // ���� TraceTag, ����˻翩�� (�Ƹ� bTraceComplex), �׸��� ������ ��ü
	//FCollisionQueryParams Params;
	

	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * m_NormalAttackRange;

	bool bResult = GetWorld()->SweepMultiByChannel(
		detectedObjects, // Ž���� ���͵�
		start, // Ž�� ���� ��ġ
		end, // Ž���� ���� ��ġ
		FQuat::Identity, // Ž���� ����� ������ ȸ��
		ECollisionChannel::ECC_GameTraceChannel2, // "Attack" Ʈ���̽� �Ҵ�� ä��.
		FCollisionShape::MakeSphere(m_NormalAttackRadius), // Ž���� ����� �⺻ ���� ����. ��ü,ĸ��,�ڽ� ���.
		Params); // Ž�� ����� ���� ���� ���� ��Ƶ� ����ü.

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * m_NormalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = m_NormalAttackRange * 0.5f + m_NormalAttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 3.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		m_NormalAttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif

	if (bResult)
	{
		for (auto& dectedObject : detectedObjects)
		{
			if (dectedObject.GetActor() != nullptr)
			{
				FDamageEvent DamageEvent;
				dectedObject.GetActor()->TakeDamage(0.0f, m_AttackInformations["NormalAttack"], GetController(), this);
			}
		}
	}


	//FString sweepStartString = " Sweep StartLocation : " + start.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *sweepStartString);

	//FString sweepEndString = " Sweep EndLocation : " + end.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *sweepEndString);

}

void AMainPlayer::initAssets()
{
	// RootCapsuleComponent
	if (GetCapsuleComponent() != nullptr)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
		GetCapsuleComponent()->SetCapsuleRadius(40.0f);
		UE_LOG(LogTemp, Warning, TEXT(" CapsuleComponent is Valid!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" CapsuleComponent is Not Valid!"));
	}

	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/MainPlayerAsset/Character/MainPlayer.MainPlayer'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	checkf(IsValid(mesh.Object), TEXT("Mesh is not Valid"));

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstance(TEXT("AnimBlueprint'/Game/MainPlayerAsset/ABP_MainPlayer.ABP_MainPlayer_C'"));
	if (animInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstance.Class);
	}
	checkf(IsValid(animInstance.Class), TEXT("animInstance is not Valid"));

	// SpringArm
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
	m_SpringArm->bDoCollisionTest = false;

	// TargetCamera
	m_TargetCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TargetCamera"));
	m_TargetCamera->SetupAttachment(m_SpringArm);


	// SwordCollider
	FTransform collisionTransform = { {0.0f, 90.0f, -2.0f}, {0.279196f, 1.998782f, 87.925328f}, {0.5f, 0.5f, 1.0f} };

	// rotation.y(pitch), rotation.z(yaw), rotation.x(roll)
	// location.x, location.y, location. z
	// scale.x,scale.y,scale.z

	//m_SwordCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollider"));
	//m_SwordCollider->SetupAttachment(GetMesh(), FName(TEXT("sword_bottom")));
	//m_SwordCollider->SetWorldTransform(collisionTransform);
	//m_SwordCollider->SetCapsuleHalfHeight(50.0f);
	//m_SwordCollider->SetCapsuleRadius(10.0f);
	//
	//m_SwordCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//m_SwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �ʿ��Ҷ��� Ű��


	// �̿� CharacterMovement Detail����

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
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString::Printf(TEXT("Is Jumping!!!!!")));
	}
}
void AMainPlayer::printLog()
{
	FVector location = GetActorLocation();
	FVector velocity = GetVelocity();

	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Green, FString::Printf(TEXT("location : %f  %f  %f"), location.X, location.Y, location.Z));
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("velocity : %f  %f  %f"), velocity.X, velocity.Y, velocity.Z));
	GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Green, FString::Printf(TEXT("velocity Length(speed) : %f"), m_CurSpeed));
}
