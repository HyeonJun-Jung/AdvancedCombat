// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item_Base.h"
#include "Weapon_Base.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API AWeapon_Base : public AItem_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle Equipment_StatusDataRow;

public:
	virtual void Interact_With_Implementation(class UInventoryComponent* InventoryComponent) override;

};
