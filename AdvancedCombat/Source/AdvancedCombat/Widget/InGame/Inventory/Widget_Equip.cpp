// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGame/Inventory/Widget_Equip.h"
#include "Component/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ACStructs.h"
#include "Components/WidgetSwitcher.h"

void UWidget_Equip::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_Equip::ShowEquipInventory(EEquipCategory InCategory)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;
	UInventoryComponent* inventory = PC->GetComponentByClass<UInventoryComponent>();
	if (!inventory) return;

	TArray<FSlotStruct> SlotArray;

	switch (InCategory)
	{
	case EEquipCategory::EEC_None:
	{
		break;
	}
	case EEquipCategory::EEC_Weapon:
	{
		inventory->GetAllSlotsOfCategory(SlotArray, 
			EItemCategory::EIC_Equipment, EEquipCategory::EEC_Weapon);
		break;
	}
	case EEquipCategory::EEC_Helmet:
	{
		inventory->GetAllSlotsOfCategory(SlotArray,
			EItemCategory::EIC_Equipment, EEquipCategory::EEC_Helmet);
		break;
	}
	case EEquipCategory::EEC_Chest:
	{
		inventory->GetAllSlotsOfCategory(SlotArray,
			EItemCategory::EIC_Equipment, EEquipCategory::EEC_Chest);
		break;
	}
	case EEquipCategory::EEC_Pants:
	{
		inventory->GetAllSlotsOfCategory(SlotArray,
			EItemCategory::EIC_Equipment, EEquipCategory::EEC_Pants);
		break;
	}
	case EEquipCategory::EEC_Glove:
	{
		inventory->GetAllSlotsOfCategory(SlotArray,
			EItemCategory::EIC_Equipment, EEquipCategory::EEC_Glove);
		break;
	}
	case EEquipCategory::EEC_Ring:
	{
		inventory->GetAllSlotsOfCategory(SlotArray,
			EItemCategory::EIC_Equipment, EEquipCategory::EEC_Ring);
		break;
	}
	case EEquipCategory::EEC_Cave:
	{
		inventory->GetAllSlotsOfCategory(SlotArray,
			EItemCategory::EIC_Equipment, EEquipCategory::EEC_Cave);
		break;
	}
	case EEquipCategory::EEC_Usable:
	{
		inventory->GetAllSlotsOfCategory(SlotArray,
			EItemCategory::EIC_Item, EEquipCategory::EEC_Usable);
		break;
	}
	default:
		break;
	}

	ShowInventory(SlotArray);
}
