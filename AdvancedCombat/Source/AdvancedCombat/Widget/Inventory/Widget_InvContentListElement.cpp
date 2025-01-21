// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Widget_InvContentListElement.h"
#include "Widget/Inventory/Inventory_ListData.h"
#include "Widget/Inventory/Widget_InvContentSlot.h"

void UWidget_InvContentListElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UInventory_ListData* data = Cast<UInventory_ListData>(ListItemObject);
	if (!data) return;

	int StartIdx = data->StartIdx;

	if (data->Items.IsValidIndex(0))
		Slot_01->UpdateSlot(StartIdx++, data->Items[0]);
	else
		Slot_01->ClearSlot();

	if (data->Items.IsValidIndex(1))
		Slot_02->UpdateSlot(StartIdx++, data->Items[1]);
	else
		Slot_02->ClearSlot();

	if (data->Items.IsValidIndex(2))
		Slot_03->UpdateSlot(StartIdx++, data->Items[2]);
	else
		Slot_03->ClearSlot();

	if (data->Items.IsValidIndex(3))
		Slot_04->UpdateSlot(StartIdx++, data->Items[3]);
	else
		Slot_04->ClearSlot();

	if (data->Items.IsValidIndex(4))
		Slot_05->UpdateSlot(StartIdx++, data->Items[4]);
	else
		Slot_05->ClearSlot();

}