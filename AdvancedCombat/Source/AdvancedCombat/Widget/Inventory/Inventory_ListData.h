// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACStructs.h"
#include "Inventory_ListData.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UInventory_ListData : public UObject
{
	GENERATED_BODY()

public:
	int StartIdx = 0;
	TArray<FSlotStruct> Items;
};
