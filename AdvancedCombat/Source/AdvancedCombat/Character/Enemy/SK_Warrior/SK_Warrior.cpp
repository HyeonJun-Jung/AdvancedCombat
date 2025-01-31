// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SK_Warrior/SK_Warrior.h"
#include "Character/CharacterBase_AnimInstance.h"
void ASK_Warrior::BeginPlay()
{
	Super::BeginPlay();

	AnimInst = Cast<UCharacterBase_AnimInstance>(GetMesh()->GetAnimInstance());
}

void ASK_Warrior::ComboAttack()
{
	if (AnimInst && AM_ComboAttack)
	{
		AnimInst->Montage_Play(AM_ComboAttack);
	}
}

void ASK_Warrior::SPAttack()
{
	if (AnimInst && AM_SPAttack)
	{
		AnimInst->Montage_Play(AM_SPAttack);
	}
}

