// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/QuickSlot/Widget_QuickSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Component/SlotComponent.h"
#include "Component/InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UWidget_QuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;
	USlotComponent* slotComp = playerController->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;
	UInventoryComponent* inventoryComp = playerController->GetComponentByClass<UInventoryComponent>();
	if (!inventoryComp) return;

	slotComp->Delegate_QuickSlotUpdated.RemoveAll(this);
	slotComp->Delegate_QuickSlotUpdated.AddDynamic(this, &UWidget_QuickSlot::SlotUpdated);
}

void UWidget_QuickSlot::SlotUpdated(EItemCategory inCategory, int inSlotIdx)
{
	if (SlotCategory != inCategory)
	{
		return;
	}

	Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);

	APlayerController* playerController = GetOwningPlayer();
	if (!playerController) return;
	USlotComponent* slotComp = playerController->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;

	switch (SlotCategory)
	{
	case EItemCategory::None:
	{
		break;
	}
	case EItemCategory::Weapon:
	{
		if (slotComp->WeaponSlots.IsValidIndex(inSlotIdx) && slotComp->WeaponSlots[inSlotIdx].ID > 1)
		{
			Image_Item->SetBrushFromTexture(slotComp->WeaponSlots[inSlotIdx].Icon);
			Image_Item->SetOpacity(1);
		}
		else
		{
			Image_Item->SetOpacity(0);
		}
		break;
	}
	case EItemCategory::Rune:
	{
		if (slotComp->RuneSlots.IsValidIndex(inSlotIdx) && slotComp->RuneSlots[inSlotIdx].ID > 1)
		{
			Image_Item->SetBrushFromTexture(slotComp->RuneSlots[inSlotIdx].Icon);
			Image_Item->SetOpacity(1);
		}
		else
		{
			Image_Item->SetOpacity(0);
		}
		break;
	}
	case EItemCategory::FragmentOfGod:
	{
		break;
	}
	case EItemCategory::UseableItem:
	{
		if (slotComp->UseableSlots.IsValidIndex(inSlotIdx) && slotComp->UseableSlots[inSlotIdx].ID > 1)
		{
			Image_Item->SetBrushFromTexture(slotComp->UseableSlots[inSlotIdx].Icon);
			Image_Item->SetOpacity(1);

			UInventoryComponent* inventoryComp = GetOwningPlayer()->GetComponentByClass<UInventoryComponent>();

			int quantity = inventoryComp->GetItemQuantityFromID(EItemCategory::UseableItem, slotComp->UseableSlots[inSlotIdx].ID);
			Text_Quantity->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_Quantity->SetText(FText::FromString(FString::FromInt(quantity)));
		}
		else
		{
			Image_Item->SetOpacity(0);
		}


		break;
	}
	case EItemCategory::Magic:
	{
		if (slotComp->MagicSlots.IsValidIndex(inSlotIdx) && !slotComp->MagicSlots[inSlotIdx].MagicName.IsNone())
		{
			Image_Item->SetBrushFromTexture(slotComp->MagicSlots[inSlotIdx].Icon);
			Image_Item->SetOpacity(1);
		}
		else
		{
			Image_Item->SetOpacity(0);
		}
		break;
	}
	default:
	{
		break;
	}
	}
}
