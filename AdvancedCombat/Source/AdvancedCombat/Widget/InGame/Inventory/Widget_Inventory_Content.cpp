// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGame/Inventory/Widget_Inventory_Content.h"
#include "Widget/InGame/Inventory/Widget_Inventory_Slot.h"
#include "Kismet/GameplayStatics.h"
#include "Component/InventoryComponent.h"
#include "Components/HorizontalBox.h"

void UWidget_Inventory_Content::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	if (!controller) return;
	UInventoryComponent* inventory = controller->GetComponentByClass<UInventoryComponent>();
	if (!inventory) return;

	int slotIdx = 0;

	TArray<UWidget*> Widgets01 = HorizontalBox_01->GetAllChildren();
	for (int i = 0; i < Widgets01.Num(); i++)
	{
		UWidget_Inventory_Slot* slot = Cast< UWidget_Inventory_Slot>(Widgets01[i]);
		if (slot)
		{
			slot->InitializeSlot(slotIdx);
			slotIdx++;
		}
	}

	TArray<UWidget*> Widgets02 = HorizontalBox_02->GetAllChildren();
	for (int i = 0; i < Widgets02.Num(); i++)
	{
		UWidget_Inventory_Slot* slot = Cast< UWidget_Inventory_Slot>(Widgets02[i]);
		if (slot)
		{
			slot->InitializeSlot(slotIdx);
			slotIdx++;
		}
	}

	TArray<UWidget*> Widgets03 = HorizontalBox_03->GetAllChildren();
	for (int i = 0; i < Widgets03.Num(); i++)
	{
		UWidget_Inventory_Slot* slot = Cast< UWidget_Inventory_Slot>(Widgets03[i]);
		if (slot)
		{
			slot->InitializeSlot(slotIdx);
			slotIdx++;
		}
	}

	TArray<UWidget*> Widgets04 = HorizontalBox_04->GetAllChildren();
	for (int i = 0; i < Widgets04.Num(); i++)
	{
		UWidget_Inventory_Slot* slot = Cast< UWidget_Inventory_Slot>(Widgets04[i]);
		if (slot)
		{
			slot->InitializeSlot(slotIdx);
			slotIdx++;
		}
	}
}
