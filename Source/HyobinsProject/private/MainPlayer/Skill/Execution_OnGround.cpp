// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Skill/Execution_OnGround.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "CharacterBase/AnimInstanceBase.h"
#include "Component/MainPlayerSkillComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/EnumTypes.h"

UExecution_OnGround::UExecution_OnGround() :
	m_ExecutionMontage(nullptr),
	m_ExecutionCameraBlendTime(1.0f),
	m_ExecutionCameraDistance(400.0f),
	m_ExecutionCameraFOV(70.0f)
{
}

void UExecution_OnGround::Initialize()
{
	Super::Initialize();

	APlayerController* PC = Cast<APlayerController>(m_Owner->GetController());
	
	
	m_OwnerAnimInstance->BindLambdaFunc_OnMontageAllEnded(TEXT("Execution_OnGround"),
	[=]()
	{
		PC->SetViewTargetWithBlend(m_Owner.Get(), m_ExecutionCameraBlendTime);
		
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
		ownerSkillComponent->SetSkillState(EMainPlayerSkillStates::Dodge_NonTargeting_OnGround);

		m_Owner->RotateToTarget(target);
		
		// 몬스터와 플레이어 정보를 가져왔다고 가정
        APlayerController* PC = Cast<APlayerController>(m_Owner->GetController());
        
        // 몬스터 좌측 지점 계산 (좌측은 -RightVector)
        FVector MonsterLocation = target->GetActorLocation();
        FVector MonsterRight = target->GetActorRightVector();
        FVector CameraPos = MonsterLocation + (MonsterRight * m_ExecutionCameraDistance);
        
        // 몬스터를 바라보는 방향 계산 (Look At)
        FRotator CameraRot = (MonsterLocation - CameraPos).Rotation();
        
        // 임시 카메라 액터 생성
       m_ExecutionCamera = PC->GetWorld()->SpawnActor<ACameraActor>(CameraPos, CameraRot);
        
        // (옵션) FOV 등 카메라 세부 설정 가능
        m_ExecutionCamera->GetCameraComponent()->SetFieldOfView(70.0f); // 예시값
		
		PC->SetViewTargetWithBlend(m_ExecutionCamera.Get(), m_ExecutionCameraBlendTime);
		
		m_OwnerAnimInstance->Montage_Play(m_ExecutionMontage,1.0f);
	}
}

bool UExecution_OnGround::CanExecuteSkill() const
{
	return Super::CanExecuteSkill() && !m_Owner->IsCrowdControlState();
}

ACharacterBase* UExecution_OnGround::findTarget()
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
