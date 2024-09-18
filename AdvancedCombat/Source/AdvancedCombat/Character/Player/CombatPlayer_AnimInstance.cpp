// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/CombatPlayer_AnimInstance.h"

void UCombatPlayer_AnimInstance::AnimNotify_AttackInputStart()
{
	Delegate_AttackInputStart.Broadcast();
}

void UCombatPlayer_AnimInstance::AnimNotify_CheckDoNextAttack()
{
	Delegate_CheckDoNextAttack.Broadcast();
}

void UCombatPlayer_AnimInstance::AnimNotify_DashAttack()
{
	Delegate_DashAttack.Broadcast();
}
