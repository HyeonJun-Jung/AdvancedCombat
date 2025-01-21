// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/CombatPlayer_AnimInstance.h"
#include "Component/EquipComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void UCombatPlayer_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Character)
	{
		EquipComponent = Character->GetComponentByClass<UEquipComponent>();
	}
}

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

void UCombatPlayer_AnimInstance::AnimNotify_ShootMagic_Staff()
{
	Delegate_ShootMagic_Staff.Broadcast();
}

void UCombatPlayer_AnimInstance::AnimNotify_ShootMagic_Hand()
{
	Delegate_ShootMagic_Hand.Broadcast();
}
