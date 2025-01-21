// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Widget_Inventory_CategorySlot.h"
#include "Subsystem/UISubsystem.h"
#include "Widget/Inventory/Widget_Inventory.h"
#include "ACGameplayTags.h"
#include "Components/Button.h"

FEventReply UWidget_Inventory_CategorySlot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
		if (!UISystem) return Reply;

		UUserWidget* widget =
			UISystem->GetWidget(FACGameplayTags::Get().UI_Widget_GameMenu_Inventory);
		if (!widget) return Reply;

		UWidget_Inventory* InventoryWidget = Cast<UWidget_Inventory>(widget);
		if (!InventoryWidget) return Reply;

		InventoryWidget->UpdateContents(Category);
	}

	return Reply;
}

