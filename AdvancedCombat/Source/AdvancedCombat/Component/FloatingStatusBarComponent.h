// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingStatusBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UFloatingStatusBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
