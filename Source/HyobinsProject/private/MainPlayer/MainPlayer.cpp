// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer/MainPlayer.h"
#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayerController.h"
#include <GameFramework/SpringArmComponent.h.>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>
#include "Utility/EnumTypes.h" 
#include "Utility/Utility.h"
#include "DrawDebugHelpers.h"


AMainPlayer::AMainPlayer() :
	m_ArmLengthTo(450.0f),
	m_ArmRotationTo(10.0f),
	m_ArmLengthSpeed(3.0f),
	m_ArmRotationSpeed(0.0f),
	m_MovdDeltaSecondsOffset(20000.0f),
	m_RotationDeltaSecondsOffset(50.0f),
	m_bIsCombated(true),
	m_bIsHit(false),
	m_bIsPressingShift(false),
	m_bIsDodgeMoving(false),
	m_bCanNextCombo(false),
	m_bIsInputOnNextCombo(false),
	m_CurNormalAttackCombo(0),
	m_NormalAttackMaxCombo(4)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMainPlayerController::StaticClass();

	this->Tags.Add(FName("MainPlayer"));

	m_WalkSpeed = 300.0f;
	m_RunSpeed = 1300.0f;

	initAssets();
	initAttackInformations("DataTable'/Game/DataAsset/AttackInformation_Player.AttackInformation_Player'");

	m_bIsInputOnNextCombo = false;
	m_bCanNextCombo = false;
	m_CurNormalAttackCombo = 0;
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0.0f, 0.0f, 100.0f));

	m_AnimInstance = Cast<UMainPlayerAnim>(GetMesh()->GetAnimInstance());
	m_AnimInstance->OnMontageEnded.AddDynamic(this, &AMainPlayer::onNormalAttackMontageEnded);

	// Notify
	m_AnimInstance->OnNormalAttackHitCheck.AddUObject(this, &AMainPlayer::onCalledNotify_NormalAttackHitCheck);
	m_AnimInstance->OnNormalAttackNextCheck.AddUObject(this, &AMainPlayer::onCalledNotify_NormalAttackNextCheck);
	m_AnimInstance->OnEndedDodgeMove.AddUObject(this, &AMainPlayer::onCalledNotify_EndedDodgeMove);
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateState();
	m_SpringArm->TargetArmLength = FMath::FInterpTo(m_SpringArm->TargetArmLength, m_ArmLengthTo, DeltaTime, m_ArmLengthSpeed);

	printLog();
}

void AMainPlayer::normalComboAttack() // 마우스좌버튼 클릭시 호출
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
		m_AnimInstance->PlayMontage("NormalAttack", 1.2f);
		m_AnimInstance->JumpToMontageSection("NormalAttack",m_CurNormalAttackCombo); // 0(비전투)에서 1로 점프
		m_bIsAttacking = true;
	}
}

void AMainPlayer::onNormalAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) // 기본공격몽타주가 끝까지 재생 되었거나, 공격 도중 키입력이 더이상 없거나
{
	m_bIsAttacking = false;
	m_bIsInputOnNextCombo = false;
	m_bCanNextCombo = false;
	m_CurNormalAttackCombo = 0;
}

void AMainPlayer::checkOverlapSwordCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//FString hitCompName = HitComp->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("hitCompName : %s"),*hitCompName);

	FHitResult hitResult;
	OtherActor->TakeDamage(0.0f, m_AttackInformations["NormalAttack"], GetController(), this);
}

void AMainPlayer::checkOverlapShieldCollisionForAttack(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->TakeDamage(0.0f, m_AttackInformations["NormalAttack"], GetController(), this);
}

void AMainPlayer::checkOverlapShieldCollisionForShield(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AMainPlayer::onCalledNotify_NormalAttackNextCheck()
{
	m_SwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShieldColliderForAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (m_CurNormalAttackCombo == 4) return;

	m_bCanNextCombo = false;
	
	if (m_bIsInputOnNextCombo) // 적절한 타이밍에 키입력 되면
	{
		updateNormalAttackStateOnStart();
		m_AnimInstance->JumpToMontageSection("NormalAttack", m_CurNormalAttackCombo);
	}
}

void AMainPlayer::onCalledNotify_NormalAttackHitCheck() // 충돌체크타이밍
{
	if (m_CurNormalAttackCombo == 3) // 방패로 공격하는 타이밍
	{
		m_ShieldColliderForAttack->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else // 검으로 공격하는 타이밍
	{
		m_SwordCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	}
}

void AMainPlayer::onCalledNotify_EndedDodgeMove()
{
	m_bIsDodgeMoving = false;
}

void AMainPlayer::updateNormalAttackStateOnStart() // 각 구간의 기본공격(연속공격이니까) 수행 후, 상태값 업데이트.
{
	rotateUsingControllerYawAndInput();
	m_bCanNextCombo = true;
	m_bIsInputOnNextCombo = false;
	m_CurNormalAttackCombo = FMath::Clamp<int32>(m_CurNormalAttackCombo + 1, 1, m_NormalAttackMaxCombo);
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
	PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &AMainPlayer::TriggerPressedSpaceBar);
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

	if (!m_bIsAttacking && !m_bIsDodgeMoving)
	{
		FVector worldDirection = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
		AddMovementInput(worldDirection, value * GetWorld()->GetDeltaSeconds() * m_MovdDeltaSecondsOffset);
	}
}

void AMainPlayer::InputVertical(float value)
{
	m_CurInputVertical = value;

	if (!m_bIsAttacking && !m_bIsDodgeMoving)
	{
		FVector worldDirection = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
		AddMovementInput(worldDirection, value * GetWorld()->GetDeltaSeconds() * m_MovdDeltaSecondsOffset);
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

void AMainPlayer::TriggerPressedSpaceBar()
{
	if (m_bIsDodgeMoving) return;

	m_bIsDodgeMoving = true;
	m_AnimInstance->StopAllMontages(0.1f);

	m_TempInputHorizontalForDodge = m_CurInputHorizontal;
	m_TempInputVerticalForDodge = m_CurInputVertical;

	if (m_bIsAttacking)
	{
		FRotator controllerRotation = GetControlRotation();
		FRotator actorRotation = GetActorRotation();

		FRotator temp = { actorRotation.Pitch, controllerRotation.Yaw, actorRotation.Roll };
		SetActorRotation(temp);
	}
	else
	{
		rotateUsingControllerYawAndInput();
		m_AnimInstance->PlayMontage("Dodge_NonCombat", 1.0f);
	}
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
	m_SwordCollider->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::checkOverlapSwordCollision);


	// ShieldCollider For Attack
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {1.0f, 1.25f, 0.35f} };

	m_ShieldColliderForAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldColliderForAttack"));
	m_ShieldColliderForAttack->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldColliderForAttack->SetWorldTransform(collisionTransform);
	m_ShieldColliderForAttack->SetCollisionProfileName(TEXT("AttackCollider"));
	m_ShieldColliderForAttack->SetGenerateOverlapEvents(true); // 블루프린트의 Generate Overlap Events에 대응되는 코드.
	m_ShieldColliderForAttack->SetNotifyRigidBodyCollision(false);
	m_ShieldColliderForAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 필요할때만 키기
	m_ShieldColliderForAttack->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::checkOverlapShieldCollisionForAttack);

	// ShieldCollider For Shield
	collisionTransform = { {0.0f, 90.0f, -10.0f}, {0.0f, 0.0f, 10.0f}, {1.0f, 1.25f, 0.1f} };

	m_ShieldColliderForShield = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldColliderForShield"));
	m_ShieldColliderForShield->SetupAttachment(GetMesh(), FName(TEXT("shield_inner")));
	m_ShieldColliderForShield->SetWorldTransform(collisionTransform);
	m_ShieldColliderForShield->SetCollisionProfileName(TEXT("HitCollider"));
	m_ShieldColliderForShield->SetGenerateOverlapEvents(true); // 블루프린트의 Generate Overlap Events에 대응되는 코드.
	m_ShieldColliderForShield->SetNotifyRigidBodyCollision(false);
	m_ShieldColliderForShield->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 필요할때만 키기


	
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
	FVector forwardVector = GetActorForwardVector();

	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Green, FString::Printf(TEXT("Location : %f  %f  %f"), location.X, location.Y, location.Z));
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("Velocity : %f  %f  %f"), velocity.X, velocity.Y, velocity.Z));
	GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Green, FString::Printf(TEXT("Forward : %f  %f  %f"), forwardVector.X, forwardVector.Y, forwardVector.Z));
	GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Green, FString::Printf(TEXT("Velocity Length(speed) : %f"), m_CurSpeed));
	GEngine->AddOnScreenDebugMessage(6, 3.f, FColor::Green, FString::Printf(TEXT("CurCombo : %d"), m_CurNormalAttackCombo));
	GEngine->AddOnScreenDebugMessage(7, 3.f, FColor::Green, FString::Printf(TEXT("is Attacking : %d"), m_bIsAttacking));
	GEngine->AddOnScreenDebugMessage(8, 3.f, FColor::Green, FString::Printf(TEXT("is DodgeMoving : %d"), m_bIsDodgeMoving));
	GEngine->AddOnScreenDebugMessage(9, 3.f, FColor::Green, FString::Printf(TEXT("is inputVertical : %d"), m_CurInputVertical));
	GEngine->AddOnScreenDebugMessage(10, 3.f, FColor::Green, FString::Printf(TEXT("is inputHorizontal : %d"), m_CurInputHorizontal));

}

void AMainPlayer::rotateUsingControllerYawAndInput()
{
	FRotator controllerRotation = GetControlRotation();
	FRotator actorRotation = GetActorRotation();
	double degree = Utility::ConvertToDegree(m_CurInputVertical, m_CurInputHorizontal);

	FRotator temp = { actorRotation.Pitch, controllerRotation.Yaw + degree, actorRotation.Roll };
	SetActorRotation(temp);
}