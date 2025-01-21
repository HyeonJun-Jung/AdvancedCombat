// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Equipment/Widget_Equipment_Slot.h"
#include "Widget/Equipment/Widget_Equipment.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Subsystem/UISubsystem.h"
#include "ACGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Component/InventoryComponent.h"
#include "Component/SlotComponent.h"

void UWidget_Equipment_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;
	USlotComponent* slotComp = playerController->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;

	slotComp->Delegate_SlotUpdated.RemoveAll(this);
	slotComp->Delegate_SlotUpdated.AddDynamic(this, &UWidget_Equipment_Slot::SlotUpdated);
}

FEventReply UWidget_Equipment_Slot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
		if (!UISystem) return Reply;

		UUserWidget* widget =
			UISystem->GetWidget(FACGameplayTags::Get().UI_Widget_GameMenu_Equipment);
		if (!widget) return Reply;

		UWidget_Equipment* EquipmentWidget = Cast<UWidget_Equipment>(widget);
		if (!EquipmentWidget) return Reply;

		EquipmentWidget->ShowEquipSelect(Category, SlotIdx);
	}

	return Reply;
}

void UWidget_Equipment_Slot::SlotUpdated(EItemCategory inCategory, int inSlotIdx)
{
	if (Category != inCategory || inSlotIdx != SlotIdx) return;

	Text_Quantity->SetVisibility(ESlateVisibility::Collapsed);

	APlayerController* playerController = GetOwningPlayer();
	if (!playerController) return;
	USlotComponent* slotComp = playerController->GetComponentByClass<USlotComponent>();
	if (!slotComp) return;

	switch (Category)
	{
	case EItemCategory::None:
	{
		break;
	}
	case EItemCategory::Weapon:
	{
		if (slotComp->WeaponSlots.IsValidIndex(inSlotIdx))
		{
			Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_Item->SetBrushFromTexture(slotComp->WeaponSlots[inSlotIdx].Icon);

			if (slotComp->WeaponSlots[inSlotIdx].ID <= 1)
				Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}
	case EItemCategory::Rune:
	{
		if (slotComp->RuneSlots.IsValidIndex(inSlotIdx))
		{
			Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_Item->SetBrushFromTexture(slotComp->RuneSlots[inSlotIdx].Icon);

			if (slotComp->RuneSlots[inSlotIdx].ID <= 1)
				Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}
	case EItemCategory::FragmentOfGod:
	{
		break;
	}
	case EItemCategory::UseableItem:
	{
		if (slotComp->UseableSlots.IsValidIndex(inSlotIdx))
		{
			Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_Item->SetBrushFromTexture(slotComp->UseableSlots[inSlotIdx].Icon);

			if (slotComp->UseableSlots[inSlotIdx].ID <= 1)
				Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}
	case EItemCategory::Magic:
	{
		if (slotComp->MagicSlots.IsValidIndex(inSlotIdx))
		{
			Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_Item->SetBrushFromTexture(slotComp->MagicSlots[inSlotIdx].Icon);

			if (slotComp->MagicSlots[inSlotIdx].MagicID < 0)
				Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

