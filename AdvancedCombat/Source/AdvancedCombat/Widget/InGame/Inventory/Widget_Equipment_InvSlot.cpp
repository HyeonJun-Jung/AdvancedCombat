// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGame/Inventory/Widget_Equipment_InvSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Component/EquipComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/InGame/Inventory/Widget_Equip.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/Character.h"

FEventReply UWidget_Equipment_InvSlot::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if (ItemInfo.ID == -1 || ItemInfo.ItemName.IsNone())
			return Reply;

		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
		UEquipComponent* EquipComp = Player->GetComponentByClass<UEquipComponent>();
		if (!EquipComp)
			return Reply;

		// Equip Item
		EquipComp->EquipItem(SlotInfo, ItemInfo);


		// Toggle UI In Equip Widget
		TArray<UUserWidget*> EquipWidget_Array;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, EquipWidget_Array,
			UWidget_Equip::StaticClass(), false);
		if (EquipWidget_Array.Num() > 0)
		{
			UWidget_Equip* EquipWidget = Cast<UWidget_Equip>(EquipWidget_Array[0]);
			if (EquipWidget)
			{
				EquipWidget->WidgetSwitcher->SetActiveWidgetIndex(0);
			}
		}
	}

	return Reply;
}
