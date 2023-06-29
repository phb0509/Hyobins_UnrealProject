// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer/MainPlayerAnim.h"
#include "MainPlayer/MainPlayer.h"
#include "Utility/EnumTypes.h"

UMainPlayerAnim::UMainPlayerAnim() 
{
	initAssets();
}

void UMainPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!m_Owner.IsValid())
	{
		m_Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	}
}

void UMainPlayerAnim::AnimNotify_checkNormalAttackHit() // 노티파이 실행 함수. 몽타주파일의 노티파이이름과 동일하게 생성해야한다.
{
	OnNormalAttackHitCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_checkNextNormalAttack()
{
	OnNormalAttackNextCheck.Broadcast();
}

void UMainPlayerAnim::AnimNotify_EndedDodgeMove()
{
	OnEndedDodgeMove.Broadcast();
}

void UMainPlayerAnim::initAssets()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> normalAttack
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/AMBP_ComboAttack1.AMBP_ComboAttack1'"));

	if (normalAttack.Succeeded())
	{
		m_Montages.Add("NormalAttack", normalAttack.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> dodge_Combat
	(TEXT("AnimMontage'/Game/MainPlayerAsset/Animations/Dodge_NonCombat.Dodge_NonCombat'"));

	if (dodge_Combat.Succeeded())
	{
		m_Montages.Add("Dodge_NonCombat", dodge_Combat.Object);
	}
}
