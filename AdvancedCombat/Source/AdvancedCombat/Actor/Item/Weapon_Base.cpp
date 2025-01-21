// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Weapon_Base.h"
#include "ACStructs.h"
#include "Component/InventoryComponent.h"

void AWeapon_Base::Interact_With_Implementation(UInventoryComponent* InventoryComponent)
{
	if (ItemDataRow.DataTable && Equipment_StatusDataRow.DataTable)
	{
		FItemStruct* itemData =
			ItemDataRow.DataTable->FindRow<FItemStruct>(ItemDataRow.RowName, ItemDataRow.RowName.ToString());

		FWeaponStruct* equipmentData =
			Equipment_StatusDataRow.DataTable->FindRow<FWeaponStruct>(Equipment_StatusDataRow.RowName, Equipment_StatusDataRow.RowName.ToString());
		
		if (itemData && equipmentData)
		{
			FSlotStruct slot = FSlotStruct();
			slot.ID = itemData->ID; slot.ItemName = itemData->ItemName; slot.Category = itemData->Category;
			slot.Quantity = 1;
			InventoryComponent->AddToInventory(slot);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeapon_Base : DataTable Is not valid."));
	}
}
