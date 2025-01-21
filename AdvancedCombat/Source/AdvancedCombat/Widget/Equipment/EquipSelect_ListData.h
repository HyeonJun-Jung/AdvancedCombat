// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACEnums.h"
#include "ACStructs.h"
#include "EquipSelect_ListData.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UEquipSelect_ListData : public UObject
{
	GENERATED_BODY()

public:
	EItemCategory Category;

	TArray<FSlotStruct> Items;
	TArray<FACMagicStruct> Magics;
};
