// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Minions/Super/SuperMinionAnim.h"

#include "Monster/Minions/Super/SuperMinion.h"

const FName USuperMinionAnim::HitMontages[4] = {"OnHit0","OnHit1","OnHit2","OnHit3"};
const FName USuperMinionAnim::GroggyMontage = "Groggy";
const FName USuperMinionAnim::DeathMontages[4] = {"Death0","Death1","Death2","Death3"};

USuperMinionAnim::USuperMinionAnim()
{
	
}

void USuperMinionAnim::AnimNotify_Test() const
{
}


