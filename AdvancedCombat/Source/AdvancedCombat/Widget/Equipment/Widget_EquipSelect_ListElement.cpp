// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Equipment/Widget_EquipSelect_ListElement.h"
#include "Widget/Equipment/EquipSelect_ListData.h"
#include "Widget/Equipment/Widget_EquipSelect_ContentSlot.h"

void UWidget_EquipSelect_ListElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UEquipSelect_ListData* data = Cast<UEquipSelect_ListData>(ListItemObject);
	if (!data) return;

	if (data->Category == EItemCategory::Magic)
	{
		if (data->Magics.IsValidIndex(0))
			Slot_01->UpdateSlot(data->Category, data->Magics[0]);

		if (data->Magics.IsValidIndex(1))
			Slot_02->UpdateSlot(data->Category, data->Magics[1]);
		else
			Slot_02->ClearSlot();

		if (data->Magics.IsValidIndex(2))
			Slot_03->UpdateSlot(data->Category, data->Magics[2]);
		else
			Slot_03->ClearSlot();

		if (data->Magics.IsValidIndex(3))
			Slot_04->UpdateSlot(data->Category, data->Magics[3]);
		else
			Slot_04->ClearSlot();

		if (data->Magics.IsValidIndex(4))
			Slot_05->UpdateSlot(data->Category, data->Magics[4]);
		else
			Slot_05->ClearSlot();
	}
	else
	{
		if (data->Items.IsValidIndex(0))
			Slot_01->UpdateSlot(data->Category, data->Items[0]);
		else
			Slot_01->ClearSlot();

		if (data->Items.IsValidIndex(1))
			Slot_02->UpdateSlot(data->Category, data->Items[1]);
		else
			Slot_02->ClearSlot();

		if (data->Items.IsValidIndex(2))
			Slot_03->UpdateSlot(data->Category, data->Items[2]);
		else
			Slot_03->ClearSlot();

		if (data->Items.IsValidIndex(3))
			Slot_04->UpdateSlot(data->Category, data->Items[3]);
		else
			Slot_04->ClearSlot();

		if (data->Items.IsValidIndex(4))
			Slot_05->UpdateSlot(data->Category, data->Items[4]);
		else
			Slot_05->ClearSlot();
	}
}
