// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ACAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UACAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/** Called on local player always. Called on server only if bReplicateInputDirectly is set on the GameplayAbility. */
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;

	/** Called on local player always. Called on server only if bReplicateInputDirectly is set on the GameplayAbility. */
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;
};
