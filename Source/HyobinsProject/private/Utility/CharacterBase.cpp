// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CharacterBase.h"
#include "Utility/AIControllerBase.h"
#include "Utility/AnimInstanceBase.h"
#include "Utility/Utility.h"
#include <Components/CapsuleComponent.h>
#include "Components/WidgetComponent.h"
#include "UI/CharacterUpperHPBar.h"
#include "HPGameInstance.h"

ACharacterBase::ACharacterBase() :
	m_WalkSpeed(200.0f),
	m_RunSpeed(400.0f),
	m_HitRecovery(1.0f),
	m_OnHitTimerTime(1.0f),
	m_DeathTimerTime(3.0f),
	m_CurSpeed(0.0f),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsAttacking(false),
	m_bIsInAir(false),
	m_bIsSuperArmor(false),
	m_bIsDeath(false),
	m_bIsHitStateTrigger(false),
	m_DeathTimerTickTime(1.0f),
	m_DeathTimerRemainingTime(3.0f),
	m_DiffuseRatio(1.0f)
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	m_StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat"));
	m_StatComponent->OnHPIsZero.AddUObject(this, &ACharacterBase::OnHPIsZero);


	// UI 생성..
	m_HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar_Widget"));
	m_HPBarWidget->SetupAttachment(GetMesh());
	m_HPBarWidget->SetRelativeLocation(FVector(0, 0, 180));
	m_HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/Monster/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		m_HPBarWidget->SetWidgetClass(UI_HUD.Class);
		m_HPBarWidget->SetDrawSize(FVector2D(150, 50));
	}
}

void ACharacterBase::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	FString temp = Tags[0].ToString() + " :: CharacterBase :: Possessedby!!";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *temp);

	m_AIControllerBase = Cast<AAIControllerBase>(newController);
	m_AnimInstanceBase = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());

	UCharacterUpperHPBar* upperHPBar = Cast<UCharacterUpperHPBar>(m_HPBarWidget->GetUserWidgetObject());
	if (upperHPBar != nullptr)
	{
		upperHPBar->BindStatComponent(m_StatComponent);
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACharacterBase* instigatorCharacter = Cast<ACharacterBase>(EventInstigator->GetPawn());
	checkf(IsValid(instigatorCharacter), TEXT("InstigatorCharacter is not Valid"));

	const FAttackInfoStruct* attackInformation = static_cast<const FAttackInfoStruct*>(&DamageEvent);
	checkf(IsValid(DamageCauser), TEXT("DamageCauser is not Valid"));

	m_StatComponent->SetDamage(attackInformation->damage);

	// 로그.
	FString log = Tags[0].ToString() + " Takes " + FString::SanitizeFloat(attackInformation->damage) + " damage from " + instigatorCharacter->Tags[0].ToString() + "::" + attackInformation->attackName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

	if (!m_bIsDeath)
	{
		m_HitDirection = Utility::GetHitDirection(this, instigatorCharacter);
		ExecHitEvent(instigatorCharacter);

		if (!m_bIsSuperArmor)
		{
			SetCommonState(EMonsterCommonStates::Hit); // 몽타주 재생 및, curState이랑 블랙보드에 Hit상태 기록.

			m_bIsAttacking = false; // 슈퍼아머같은 상태가 아니면 피격시 강제 온힛상태가 되니까 attacking을 false로 해줘야 한다.

			FVector dirToInstigator = instigatorCharacter->GetActorLocation() - this->GetActorLocation();
			dirToInstigator.Normalize();
			this->SetActorLocation(GetActorLocation() + dirToInstigator * -1 * attackInformation->knockBackDistance, false);

			// Timer Setting.
			m_OnHitTimerTime = m_HitRecovery * attackInformation->knockBackTime;

			if (GetWorldTimerManager().IsTimerActive(m_OnHitTimerHandle))
			{
				GetWorldTimerManager().ClearTimer(m_OnHitTimerHandle);
			}

			GetWorldTimerManager().SetTimer(m_OnHitTimerHandle, this, &ACharacterBase::OnHitTimerEnded, m_OnHitTimerTime, true); // OnHitTimeEnded는 알아서 오버라이드되서 호출됨.
		}
	}
	
	return FinalDamage;
}

void ACharacterBase::OnHPIsZero()
{
	m_bIsDeath = true;
	m_bIsAttacking = false;
	Die();
}

void ACharacterBase::OnCalledDeathMontageEndedNotify()
{
	ExecDeathEvent();

	// 액터풀에 반환하기위한 비활성화타이머
	GetWorldTimerManager().SetTimer(m_DeActivateTimerHandle, this, &ACharacterBase::DeActivate, m_DeathTimerTime, true);
}

void ACharacterBase::initAttackInformations(FString path)
{
	auto HPGameInstance = Cast<UHPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	HPGameInstance->InitAttackInformations(path, m_AttackInformations);
}