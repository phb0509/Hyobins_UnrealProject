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

