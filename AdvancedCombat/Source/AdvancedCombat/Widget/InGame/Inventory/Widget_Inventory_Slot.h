// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Inventory_Slot.generated.h"

class UImage;
class UTextBlock;
class UInventoryComponent;
UCLASS()
class ADVANCEDCOMBAT_API UWidget_Inventory_Slot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Item;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Quantity;

public:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

public:
	void InitializeSlot(int InSlotIdx);

	UFUNCTION()
	void InventoryUpdated();
	
public:
	int SlotIdx = 0;

	UPROPERTY()
	UInventoryComponent* Inventory;
};
