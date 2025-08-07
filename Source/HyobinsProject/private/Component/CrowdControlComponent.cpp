
#include "Component/CrowdControlComponent.h"
#include "Monster/Monster.h"
#include "CharacterBase/AnimInstanceBase.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "CharacterBase/AIControllerBase.h"
#include "Utility/EnumTypes.h"
#include "Utility/CustomStructs.h"


UCrowdControlComponent::UCrowdControlComponent() :
	m_Owner(nullptr),
	m_CrowdControlSetting({}),
	m_CharacterMovementComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECrowdControlType"), true);
	if (enumPtr != nullptr)
	{
		for (int i = 0; i < enumPtr->NumEnums(); ++i)
		{
			ECrowdControlType state = static_cast<ECrowdControlType>(enumPtr->GetValueByIndex(i));

			if (state == ECrowdControlType::None)
			{
				continue;
			}
			
			FOnCrowdControl_Start_Delegate startDelegate;
			m_CrowdControlStartDelegates.Add(state,startDelegate);
		}
	}
}

void UCrowdControlComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_Owner = Cast<ACharacterBase>(GetOwner());
	m_OwnerAnimInstance = Cast<UAnimInstanceBase>(m_Owner->GetMesh()->GetAnimInstance());
	
	m_CrowdControlStartDelegates[ECrowdControlType::Knockback].AddUObject(this, &UCrowdControlComponent::TakeAttack_Knockback);
	m_CrowdControlStartDelegates[ECrowdControlType::Down].AddUObject(this, &UCrowdControlComponent::TakeAttack_Down);
    m_CrowdControlStartDelegates[ECrowdControlType::Airborne].AddUObject(this, &UCrowdControlComponent::TakeAttack_Airborne);

	m_OnwerAIController = Cast<AAIController>(GetOwner()->GetInstigatorController());
}

void UCrowdControlComponent::ApplyCrowdControl(AActor* instigator, const FHitInformation& attackInfo)
{
	if (m_OnwerAIController != nullptr) 
	{
		m_OnwerAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsCrowdControlState"), true);
	}
	
	m_CrowdControlStartDelegates[attackInfo.crowdControlType].Broadcast(instigator, attackInfo);


	if (attackInfo.knockBackDistance > 0.0f)
	{
		applyKnockback(attackInfo);
	}
}

void UCrowdControlComponent::Groggy()
{
	ClearCrowdControlTimerHandle();
	SetCrowdControlState(ECrowdControlType::Groggy);
	playCrowdControlMontage(ECrowdControlType::Groggy, 0);

	FTimerHandle timerHandle;
	m_Owner->GetWorldTimerManager().SetTimer
		(
			timerHandle,
			[this]()
			{
				if (!m_Owner->IsDead())
				{
					m_OwnerAnimInstance->StopAllMontages(0.0f);
					OnEndedGroggy.Broadcast();
				}
			},
		m_CrowdControlSetting.groggyTime,
		false);
}

void UCrowdControlComponent::Execution()
{
	
}

void UCrowdControlComponent::applyKnockback(const FHitInformation& hitInfo)
{
	AActor* instigator = hitInfo.instigator.Get();
	
	FVector dirToInstigator = instigator->GetActorLocation() - m_Owner->GetActorLocation(); 
	dirToInstigator.Normalize();
	dirToInstigator *= -1 * hitInfo.knockBackDistance;
	dirToInstigator.Z = 0.0f;
	
	hitInfo.hitActor->SetActorLocation(m_Owner->GetActorLocation() + dirToInstigator, false);
}

void UCrowdControlComponent::TakeAttack_Knockback(AActor* instigator, const FHitInformation& attackInfo)
{
	if (m_CurCrowdControlState == ECrowdControlType::Down) // �ٿ� ����
	{
		CallTimer_CheckOnGround();
	}
	else if (m_CurCrowdControlState == ECrowdControlType::Airborne) // ��� ����
	{
		playCrowdControlMontage(ECrowdControlType::Airborne, attackInfo.hitDirection);

		DisableMovementComponentForDuration(0.2f);
		CallTimer_CheckOnGround();
	} 
	else // KnockbackOnGround
	{
		SetCrowdControlState(ECrowdControlType::Knockback);
		playCrowdControlMontage(ECrowdControlType::Knockback, attackInfo.hitDirection);

		GetOwner()->GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&UCrowdControlComponent::OnCalledTimer_KnockbackOnStanding_End,
			attackInfo.crowdControlTime, false);
	}

	UAnimMontage* curActivateMontage = m_OwnerAnimInstance->GetCurrentActiveMontage();
	if (curActivateMontage != nullptr)
	{
		m_LastPlayedOnHitMontageName = curActivateMontage->GetFName();
	}
}

void UCrowdControlComponent::OnCalledTimer_KnockbackOnStanding_End() // �˹�CC�ð� ���� �� ȣ��
{
	if (m_Owner->IsDead())
	{
		ClearCrowdControlTimerHandle();
		return;
	}
	
	FName curMontageName = "";
	
	if (m_OwnerAnimInstance->GetCurrentActiveMontage() != nullptr)
	{
		curMontageName = m_OwnerAnimInstance->GetCurrentActiveMontage()->GetFName();
	}
	
	if (curMontageName == m_LastPlayedOnHitMontageName) // �ǰݻ��¸� �����ϰ� �־������ ( �� �Լ��� ȣ���ѽ������� ������ Knockback ��Ÿ�� ������̶�� )
	{
		m_OwnerAnimInstance->StopAllMontages(0.0f); // autoBlend�� false��, �̷��� ����� ����� ����.
	}
	
	SetCrowdControlState(ECrowdControlType::None);
}

void UCrowdControlComponent::TakeAttack_Down(AActor* instigator, const FHitInformation& attackInfo)
{
	if (m_CurCrowdControlState == ECrowdControlType::Airborne) // ��� ����
	{
		playCrowdControlMontage(ECrowdControlType::Airborne, attackInfo.hitDirection);
		DisableMovementComponentForDuration(0.2f); 
	}
	else
	{
		SetCrowdControlState(ECrowdControlType::Down);
		playCrowdControlMontage(ECrowdControlType::Down, attackInfo.hitDirection);
		
		UAnimMontage* downMontage = m_CrowdControlSetting.crowdControlMontages[ECrowdControlType::Down].montages[0];
		const float downPlayTime = m_OwnerAnimInstance->GetMontagePlayTime(downMontage) + 0.2f; // �ٿ��Ÿ������ð��� CC�ð����� ª����츦 ����� ������
		
		GetOwner()->GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&UCrowdControlComponent::GetUp,
			attackInfo.crowdControlTime > downPlayTime ? attackInfo.crowdControlTime : downPlayTime,
			false);
	}
}

void UCrowdControlComponent::CallTimer_CheckOnGround()
{
	GetOwner()->GetWorldTimerManager().SetTimer(m_CrowdControlTimerHandle,
		this,
			&UCrowdControlComponent::CheckOnGround,	
			GetWorld()->DeltaTimeSeconds,
			true,
			-1);
}

void UCrowdControlComponent::CheckOnGround() // OnGround���� ƽ���� ȣ��Ǿ����� �Լ�.
{
	if (m_Owner->IsDead())
	{
		ClearCrowdControlTimerHandle();
		return;
	}

	if (getCharacterMovementComponent()->IsMovingOnGround()) 
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
		
		playCrowdControlMontage(ECrowdControlType::Down, 0);
		SetCrowdControlState(ECrowdControlType::Down);

		UAnimMontage* downMontage = m_CrowdControlSetting.crowdControlMontages[ECrowdControlType::Down].montages[0];
		const float downPlayTime = m_OwnerAnimInstance->GetMontagePlayTime(downMontage) + 0.2f;

		GetOwner()->GetWorldTimerManager().SetTimer(
			m_CrowdControlTimerHandle,
			this,
			&UCrowdControlComponent::GetUp,
			m_LastCrowdControlTime > downPlayTime ? m_LastCrowdControlTime : downPlayTime,
			false);
	}
}

void UCrowdControlComponent::GetUp()
{
	UAnimMontage* getUpMontage = m_CrowdControlSetting.getUpMontage;
	m_OwnerAnimInstance->Montage_Play(getUpMontage);
	
	const float getupPlayTime = m_OwnerAnimInstance->GetMontagePlayTime(getUpMontage) + 0.2f;
	
	GetOwner()->GetWorldTimerManager().SetTimer(
		m_CrowdControlTimerHandle,
		[this]()
		{
			SetCrowdControlState(ECrowdControlType::None);
		},
		getupPlayTime,
		false);
}

void UCrowdControlComponent::TakeAttack_Airborne(AActor* instigator, const FHitInformation& attackInfo)
{
	FVector airbornePower = {0.0f, 0.0f, attackInfo.airbornePower};

	if (m_CurCrowdControlState == ECrowdControlType::Down) // �ٿ� ����
	{
		airbornePower.Z /= 2; // �ٿ���¿����� ����� �������� ���δ�.
		playCrowdControlMontage(ECrowdControlType::Down, attackInfo.hitDirection);
	}
	else 
	{
		playCrowdControlMontage(ECrowdControlType::Airborne, attackInfo.hitDirection);
		SetCrowdControlState(ECrowdControlType::Airborne);
	}

	FVector launchVelocity = airbornePower;
	m_Owner->LaunchCharacter(launchVelocity, true, true);

	CallTimer_CheckOnGround();
}

void UCrowdControlComponent::DisableMovementComponentForDuration(float duration)
{
	getCharacterMovementComponent()->Deactivate();

	FTimerHandle activateTimer;
	GetOwner()->GetWorldTimerManager().SetTimer(activateTimer,
		[this]()
		{
			getCharacterMovementComponent()->Activate();
		},
			duration,
			false);
}

void UCrowdControlComponent::ClearCrowdControlTimerHandle()
{
	m_Owner->GetWorldTimerManager().ClearTimer(m_CrowdControlTimerHandle);
}


UCharacterMovementComponent* UCrowdControlComponent::getCharacterMovementComponent()
{
	if (m_CharacterMovementComponent == nullptr)
	{
		m_CharacterMovementComponent = m_Owner->GetCharacterMovement();
	}
	
	return m_CharacterMovementComponent.Get();
}

void UCrowdControlComponent::playCrowdControlMontage(const ECrowdControlType crowdControlType, const int32 hitDirection)
{
	if (!m_CrowdControlSetting.crowdControlMontages.Contains(crowdControlType))
	{
		return;
	}
	
	auto montages = m_CrowdControlSetting.crowdControlMontages[crowdControlType].montages;

	if (montages.Num() >= 1) // �ּ� 1���̻�������
	{
		if (montages.Num() > hitDirection)
		{
			m_OwnerAnimInstance->Montage_Play(montages[hitDirection]);
		}
		else
		{
			m_OwnerAnimInstance->Montage_Play(montages[0]);
		}
	}

	// �ǰ� ������
	// UAnimMontage* curMontage = m_AnimInstanceBase->GetMontage(montageName);
	// m_AnimInstanceBase->Montage_SetPlayRate(curMontage,m_OnHitPlayRate);
	//
	// FTimerHandle timer;
	// GetWorldTimerManager().SetTimer
	// ( 
	// 	timer,
	// 	[=]()
	// 	{
	// 		m_AnimInstanceBase->Montage_SetPlayRate(curMontage,1.0f);
	// 	},
	// 	m_GameSpeedDelay,
	// 	false);
}

void UCrowdControlComponent::BreakCrowdControlState()
{
	ClearCrowdControlTimerHandle();
	SetCrowdControlState(ECrowdControlType::None);
}

bool UCrowdControlComponent::IsCrowdControlState() const
{
	return m_CurCrowdControlState != ECrowdControlType::None;
}

void UCrowdControlComponent::SetCrowdControlState(ECrowdControlType crowdControlType)
{
	m_CurCrowdControlState = crowdControlType;

	if (m_OnwerAIController != nullptr)
	{
		m_OnwerAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsCrowdControlState"), crowdControlType != ECrowdControlType::None);
	}
}





