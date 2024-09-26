// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ACPlayerController.h"
#include "Component/InventoryComponent.h"

AACPlayerController::AACPlayerController()
{
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("Inventory");
}
