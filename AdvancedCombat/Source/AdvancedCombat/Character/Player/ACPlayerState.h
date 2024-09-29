// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ACPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API AACPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AACPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UACAttributeSet_Base* GetAttributeSet() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UACAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UACAttributeSet_Base> AttributeSet;
};
