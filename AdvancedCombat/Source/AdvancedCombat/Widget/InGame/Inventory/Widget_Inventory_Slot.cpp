// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGame/Inventory/Widget_Inventory_Slot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Component/InventoryComponent.h"
#include "ACEnums.h"
#include "ACStructs.h"

void UWidget_Inventory_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	if (!controller) return;
	Inventory = controller->GetComponentByClass<UInventoryComponent>();
	if (!Inventory) return;

	Inventory->Delegate_InventoryUpdated.AddUObject(this, &UWidget_Inventory_Slot::InventoryUpdated);

	Image_Item->SetVisibility(ESlateVisibility::Collapsed);
	TextBlock_Quantity->SetVisibility(ESlateVisibility::Collapsed);
}

bool UWidget_Inventory_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return true;
}

void UWidget_Inventory_Slot::InitializeSlot(int InSlotIdx)
{
	SlotIdx = InSlotIdx;
}

void UWidget_Inventory_Slot::InventoryUpdated()
{
	if (!Inventory) return;
	TArray<FSlotStruct>& Contents = Inventory->GetContents();
	if (Contents.IsValidIndex(SlotIdx) && Contents[SlotIdx].Quantity > 0)
	{
		Image_Item->SetVisibility(ESlateVisibility::Visible);
		TextBlock_Quantity->SetVisibility(ESlateVisibility::Visible);

		// Image_Item->SetBrushFromTexture(Contents[SlotIdx].)
	}
}
