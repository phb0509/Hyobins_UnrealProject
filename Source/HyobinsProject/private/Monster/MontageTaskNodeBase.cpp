// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MontageTaskNodeBase.h"

UMontageTaskNodeBase::UMontageTaskNodeBase()
{
}

uint16 UMontageTaskNodeBase::GetInstanceMemorySize() const
{
	return sizeof(FInstanceNode);
}
