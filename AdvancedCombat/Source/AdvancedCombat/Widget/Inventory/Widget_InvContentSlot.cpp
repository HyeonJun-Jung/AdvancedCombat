// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Widget_InvContentSlot.h"
#include "Widget/Inventory/Widget_Inventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Component/InventoryComponent.h"
#include "Kismet/KismetTextLibrary.h"
#include "Subsystem/UISubsystem.h"
#include "Subsystem/InventorySubsystem.h"
#include "ACGameplayTags.h"

void UWidget_InvContentSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_InvContentSlot::UpdateSlot(int slotIdx, const FSlotStruct& inData)
{
	SlotIdx = slotIdx;
	SlotData = inData;

	UInventorySubsystem* invSystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	const FItemStruct* item = invSystem->GetItemInfo(SlotData.ID);
	if (!item)
		return;

	if (inData.Quantity > 0)
	{
		Image_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image_Item->SetBrushFromTexture(item->Icon);
		Text_Quantity->SetText(UKismetTextLibrary::Conv_IntToText(inData.Quantity));
	}
	else
	{
		Image_Item->SetVisibility(ESlateVisibility::Collapsed);
		Text_Quantity->SetText(FText::GetEmpty());
	}
}

void UWidget_InvContentSlot::ClearSlot()
{
	Image_Item->SetVisibility(ESlateVisibility::Collapsed);
	Text_Quantity->SetText(FText::GetEmpty());
}

void UWidget_InvContentSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	UUISubsystem* UISystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UISystem) return;

	UUserWidget* widget = UISystem->GetWidget(FACGameplayTags::Get().UI_Widget_GameMenu_Inventory);
	if (!widget) return;
	UWidget_Inventory* InventoryWidget = Cast<UWidget_Inventory>(widget);
	if (!InventoryWidget) return;

	InventoryWidget->UpdateDescription(SlotData);

	/*if (WidgetType == EWidgetClassType::Inventory)
	{
		UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_Inventory);
		if (!widget) return;
		UWidget_Inventory* InventoryWidget = Cast<UWidget_Inventory>(widget);
		if (!InventoryWidget) return;

		InventoryWidget->UpdateDescription(SlotData);
	}
	else if (WidgetType == EWidgetClassType::EquipUpgrade)
	{
		UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_UpgradeEquipment);
		if (!widget) return;
		UWidget_UpgradeEquipment* UpgradeEquipmentWidget = Cast<UWidget_UpgradeEquipment>(widget);
		if (!UpgradeEquipmentWidget) return;

		UpgradeEquipmentWidget->UpdateDescription(SlotData);
	}*/
}

FEventReply UWidget_InvContentSlot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	//{
	//	// Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	//	if (WidgetType == EWidgetClassType::EquipUpgrade && SlotData.Quantity > 0)
	//	{
	//		UUISubsystem* UISystem = GetWorld()->GetGameInstance()->GetSubsystem<UUISubsystem>();
	//		if (!UISystem) return Reply;
	//		UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_GameMenu_UpgradeEquipment);
	//		if (!widget) return Reply;
	//		UWidget_UpgradeEquipment* UpgradeEquipmentWidget = Cast<UWidget_UpgradeEquipment>(widget);
	//		if (!UpgradeEquipmentWidget) return Reply;

	//		UpgradeEquipmentWidget->ShowConfirmWidget(SlotData);
	//	}

	//}

	return Reply;
}

