// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AnimInstanceBase.h"


UAnimInstanceBase::UAnimInstanceBase() :
	m_CurSpeed(0.0f),
	m_bIsIdle(true),
	m_bIsWalking(false),
	m_bIsRunning(false),
	m_bIsInAir(false)
{

}

void UAnimInstanceBase::PlayOnHitMontage(int index)
{
	Montage_Play(m_OnHitMontages[index], 1.0f);
}

void UAnimInstanceBase::PlayDeathMontage(int index)
{
	Montage_Play(m_DeathMontages[index], 1.0f);
}