// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACEnums.h"
#include "ACStructs.h"
#include "Widget_Equip.generated.h"

class UWidgetSwitcher;

UCLASS()
class ADVANCEDCOMBAT_API UWidget_Equip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UWidgetSwitcher* WidgetSwitcher;

public:
	virtual void NativeConstruct() override;

public:
	void ShowEquipInventory(EEquipCategory InCategory);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowInventory(const TArray<FSlotStruct>& Slots);
	
};
