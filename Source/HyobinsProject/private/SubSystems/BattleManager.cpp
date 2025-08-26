// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/BattleManager.h"
#include "Interfaces/Damageable.h"
#include "SubSystems/DataManager.h"
#include "PlayableCharacter/PlayableCharacter.h"
#include "Component/StatComponent.h"
#include "Components/SphereComponent.h"
#include "Monster/Monster.h"

void UBattleManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UBattleManager::Attack(ACharacterBase* attacker, const FName& attackName, AActor* target, const FVector& causerLocation)
{
	IDamageable* damageableActor = Cast<IDamageable>(target);

	if (damageableActor == nullptr)
	{
		return;
	}

	if (attacker->IsA(APlayableCharacter::StaticClass()))
	{
		OnPlayerAttack.Broadcast();
	}
	
	const FAttackInformation* attackInfo = getDataManager()->GetAttackInformation(attacker->GetClass(), attackName);
	
	const bool bIsCriticalAttack = FMath::FRandRange(0.0f, 100.0f) <= attacker->GetStatComponent()->GetCriticalAttackChance();
	const float damage = (attacker->GetStatComponent()->GetDefaultDamage() * attackInfo->damageRatio) * (bIsCriticalAttack ? 2.0f : 1.0f);
	
	damageableActor->OnDamage(damage, bIsCriticalAttack, attackInfo, attacker, causerLocation);
} 

FCollisionShape UBattleManager::MakeCollisionShape(const UShapeComponent* sweepCollider) const
{
	if (auto* Sphere = Cast<USphereComponent>(sweepCollider))
	{
		return FCollisionShape::MakeSphere(Sphere->GetScaledSphereRadius());
	}

	if (auto* Box = Cast<UBoxComponent>(sweepCollider))
	{
		return FCollisionShape::MakeBox(Box->GetScaledBoxExtent());
	}
	
	if (auto* Capsule = Cast<UCapsuleComponent>(sweepCollider))
	{
		return FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight());
	}

	return FCollisionShape::MakeSphere(10.0f);
}

FCollisionObjectQueryParams UBattleManager::MakeCollisionObjectParams(const ACharacterBase* owner) const
{
	FCollisionObjectQueryParams collisionObjectParams;
	
	if (const APlayableCharacter* playableCharacter = Cast<APlayableCharacter>(owner))
	{
		collisionObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1); // 몬스터옹 HitCollider.
	}
	else if (const AMonster* monster = Cast<AMonster>(owner))
	{
		collisionObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel5); // 플레이어용 HitCollider.
	}

	return collisionObjectParams;
}

const UDataManager* UBattleManager::getDataManager() 
{
	if (m_DataManager != nullptr)
	{
		return m_DataManager.Get();
	}
	
	m_DataManager = GetWorld()->GetGameInstance()->GetSubsystem<UDataManager>();
	
	return m_DataManager.Get();
}

