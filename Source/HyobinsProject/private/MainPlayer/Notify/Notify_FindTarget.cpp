// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/Notify/Notify_FindTarget.h"
#include "MainPlayer/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "MotionWarpingComponent.h"

UNotify_FindTarget::UNotify_FindTarget()
{
}

void UNotify_FindTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AMainPlayer* owner = Cast<AMainPlayer>(MeshComp->GetOwner());

	if (owner != nullptr)
	{
		const FVector startLocation = owner->GetActorLocation();
		const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)};
		const TArray<AActor*> IgnoreActors = {owner};
		TArray<AActor*> overlappedActors;
		
		UKismetSystemLibrary::SphereOverlapActors(
			MeshComp->GetWorld(),
			startLocation,
			1000.0f, // 구체 반지름
			ObjectTypes,
			nullptr,
			IgnoreActors,
			overlappedActors);

		if (overlappedActors.Num() > 0)
		{
			for (AActor* overlappedEnemy : overlappedActors)
			{
				ACharacterBase* target = Cast<ACharacterBase>(overlappedEnemy);
				
				FVector targetVector = target->GetActorLocation() - owner->GetActorLocation();
				targetVector.Z = 0.0f;
				owner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
					TEXT("Forward"), target->GetActorLocation(), targetVector.Rotation());
				
				break;
			}
		}
	}
}

