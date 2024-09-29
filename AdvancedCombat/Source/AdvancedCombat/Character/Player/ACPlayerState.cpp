// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ACPlayerState.h"
#include "Character/Ability/ACAbilitySystemComponent.h"
#include "Character/Ability/ACGameplayAbility_Base.h"
#include "Character/Ability/ACGameplayEffect_Base.h"
#include "Character/Ability/AttributeSets/ACAttributeSet_Base.h"

AACPlayerState::AACPlayerState()
{
	ASC = CreateDefaultSubobject<UACAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UACAttributeSet_Base>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AACPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

UACAttributeSet_Base* AACPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
