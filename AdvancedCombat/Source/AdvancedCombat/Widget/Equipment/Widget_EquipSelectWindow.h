// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACStructs.h"
#include "ACEnums.h"
#include "Widget_EquipSelectWindow.generated.h"

class UTextBlock;
class UImage;
class UOverlay;
class UWidgetSwitcher;
class UWidget_EquipSelect_ContentList;

UCLASS()
class ADVANCEDCOMBAT_API UWidget_EquipSelectWindow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void UpdateContents(EItemCategory Category);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDescription_Item(const FSlotStruct& SlotData);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDescription_Magic(const FACMagicStruct& SlotData);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDescription_Weapon(const FSlotStruct& SlotData);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDescription_Rune(const FSlotStruct& SlotData);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDescription_Fragment(const FSlotStruct& SlotData);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateStatusInfo();

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UTextBlock* Text_Category;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UTextBlock* Text_SelectedItem;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UImage* Image_Item;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UWidget_EquipSelect_ContentList* ContentList;

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UOverlay* Overlay_Item;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UOverlay* Overlay_Weapon;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UOverlay* Overlay_Rune;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UOverlay* Overlay_Fragment;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UOverlay* Overlay_Magic;
};
