// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Inventory_Content.generated.h"

class UHorizontalBox;

UCLASS()
class ADVANCEDCOMBAT_API UWidget_Inventory_Content : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_01;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_02;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_03;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox_04;

private:
};
