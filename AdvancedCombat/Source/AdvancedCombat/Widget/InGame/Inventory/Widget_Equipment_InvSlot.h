// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACStructs.h"
#include "Widget_Equipment_InvSlot.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UWidget_Equipment_InvSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlotStruct SlotInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStruct ItemInfo;
	
public:
	UFUNCTION(BlueprintCallable)
	FEventReply RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
};
