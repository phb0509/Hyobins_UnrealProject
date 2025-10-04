// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Notify/Notify_FindTarget.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "MotionWarpingComponent.h"

UNotify_FindTarget::UNotify_FindTarget() :
	m_CollisionRadius(1000.0f)
{
}

void UNotify_FindTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayableCharacter* owner = Cast<APlayableCharacter>(MeshComp->GetOwner());

	if (owner != nullptr)
	{
		AActor* lockOnTarget = owner->GetCurLockOnTarget();
		
		if (lockOnTarget != nullptr && owner->IsLockOnMode() && owner->IsWithInRange(lockOnTarget, m_CollisionRadius))
		{
			owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
				TEXT("Forward"),
				lockOnTarget->GetActorLocation(),
				owner->GetDirectionToTarget(lockOnTarget).Rotation());
		}
		else
		{
			const FVector startLocation = owner->GetActorLocation();
			const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)};
			const TArray<AActor*> IgnoreActors = {owner};
			TArray<AActor*> overlappedActors;
		
			UKismetSystemLibrary::SphereOverlapActors(
				MeshComp->GetWorld(),
				startLocation,
				m_CollisionRadius, // 구체 반지름
				ObjectTypes,
				nullptr,
				IgnoreActors,
				overlappedActors);

			if (overlappedActors.Num() > 0)
			{
				ACharacterBase* target = Cast<ACharacterBase>(overlappedActors[0]);

				if (target != nullptr)
				{
					owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
					TEXT("Forward"),
					target->GetActorLocation(),
					owner->GetDirectionToTarget(target).Rotation());
				}
			}
		}
	}
}

