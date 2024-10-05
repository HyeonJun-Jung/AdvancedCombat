// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGame/Inventory/Widget_EquipSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/InGame/Inventory/Widget_Equip.h"

void UWidget_EquipSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UWidget_EquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return true;
}

FEventReply UWidget_EquipSlot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		TArray<UUserWidget*> EquipWidget_Array;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, EquipWidget_Array, UWidget_Equip::StaticClass(), false);
		if (EquipWidget_Array.Num() > 0 )
		{
			UWidget_Equip* EquipWidget = Cast<UWidget_Equip>(EquipWidget_Array[0]);
			if (EquipWidget)
			{
				EquipWidget->ShowEquipInventory(EquipSlot_Category);
			}
		}
	}

	return Reply;
}
