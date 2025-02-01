// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_FloatingStatusBar.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UWidget_FloatingStatusBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealthPercentage(float InValue);

	UFUNCTION(BlueprintImplementableEvent)
	void SetCharacterName(const FText& InName);

};
