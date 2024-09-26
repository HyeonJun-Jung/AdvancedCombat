// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API AACPlayerController : public APlayerController
{
	GENERATED_BODY()

	AACPlayerController();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;
};
