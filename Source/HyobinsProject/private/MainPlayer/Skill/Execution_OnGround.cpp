// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Execution_OnGround.h"
#include "CineCameraActor.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/EnumTypes.h"

UExecution_OnGround::UExecution_OnGround() :
	m_ExecutionMontage(nullptr),
	m_CameraBlendTime(1.0f),
	m_CameraDistance(400.0f),
	m_CameraFOV(70.0f),
	m_CameraLocationLeftOffset(0.0f)
{
}

void UExecution_OnGround::Initialize()
{
	Super::Initialize();

	check(m_ExecutionMontage != nullptr);
	
	APlayerController* pc = Cast<APlayerController>(m_Owner->GetController());
	check(pc != nullptr);
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Execution_OnGround"),
	[=]()
	{
		pc->SetViewTargetWithBlend(m_Owner.Get(), 1.0f);
		
		m_ExecutionCamera->Destroy();
	});
}

void UExecution_OnGround::Execute()  
{
	Super::Execute();
	
	ACharacterBase* target = findTarget();

	if (target != nullptr && target->IsGroggy())
	{
		UMainPlayerSkillComponent* ownerSkillComponent = Cast<UMainPlayerSkillComponent>(m_OwnerSkillComponent);
		check(ownerSkillComponent != nullptr);
		
		ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::Dodge_NonTargeting_OnGround);

		m_Owner->RotateToTarget(target);
		
		playExecutionCamera();
		
		m_OwnerAnimInstance->Montage_Play(m_ExecutionMontage,1.0f);
	}
}

ACharacterBase* UExecution_OnGround::findTarget() const
{
	const FVector startLocation = m_Owner->GetActorLocation() + m_Owner->GetActorForwardVector() * 60.0f;
	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)};
	const TArray<AActor*> ignoreActors = {};
	TArray<AActor*> overlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(m_Owner->GetWorld(),
		startLocation,
		200.0f, // 구체 반지름
		objectTypes,
		nullptr,
		ignoreActors,
		overlappedActors);
	
	if (overlappedActors.Num() > 0)
	{
		for (AActor* overlappedEnemy : overlappedActors)
		{
			if (IsValid(overlappedEnemy))
			{
				return Cast<ACharacterBase>(overlappedEnemy);
			}
		}
	}	
	
	return nullptr;
}

void UExecution_OnGround::playExecutionCamera()
{
	APlayerController* pc = Cast<APlayerController>(m_Owner->GetController());
	check(pc != nullptr);
        
	// 카메라 위치 셋팅. (플레이어 좌측정면에서 볼 수 있게)
	const FVector ownerLocation = m_Owner->GetActorLocation();
	FVector cameraInverseForward = m_Owner->GetActorRightVector() * -1.0f;
	cameraInverseForward.Z = 0.0f;
	FVector cameraLocation = ownerLocation + (cameraInverseForward * m_CameraDistance);
	FRotator cameraRotation = (ownerLocation - cameraLocation).Rotation();
	
	m_ExecutionCamera = pc->GetWorld()->SpawnActor<ACameraActor>(cameraLocation, cameraRotation);

	FVector offset = m_ExecutionCamera->GetActorRightVector() * -1.0f * m_CameraLocationLeftOffset;
	cameraLocation += offset;
	m_ExecutionCamera->SetActorLocation(cameraLocation);
	
	m_ExecutionCamera->GetCameraComponent()->SetFieldOfView(m_CameraFOV);
	m_ExecutionCamera->GetCameraComponent()->AspectRatio = 2.0f;
		
	pc->SetViewTargetWithBlend(m_ExecutionCamera.Get(), m_CameraBlendTime);
}

bool UExecution_OnGround::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() && !m_Owner->IsCrowdControlState();
}