// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACEnums.h"
#include "Widget_EquipSlot.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UWidget_EquipSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEquipCategory EquipSlot_Category = EEquipCategory::EEC_None;
	
public:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

public:
	UFUNCTION(BlueprintCallable)
	FEventReply RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

};
