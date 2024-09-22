// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/State/CombatState.h"
#include "Character/Player/CombatPlayer_AnimInstance.h"
#include "Character/Player/CombatPlayerCharacter.h"

UCombatState::UCombatState()
{
}

UCombatState::~UCombatState()
{
}

void UCombatState::EnterState(ACombatPlayerCharacter* InCombatCharacter)
{
	CombatCharacter = InCombatCharacter;
	AnimInst = InCombatCharacter->GetCombatAnimInst();
	PlayerController = Cast<APlayerController>(InCombatCharacter->GetController());
}
