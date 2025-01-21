// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_GameMenu_Button.generated.h"

UENUM(BlueprintType)
enum class EGameMenuCategory : uint8
{
	Inventory		UMETA(DisplayName = "Inventory"),
	Equipment		UMETA(DisplayName = "Equipment"),
	Setting			UMETA(DisplayName = "Setting"),
	Quit			UMETA(DisplayName = "Quit"),
	Map				UMETA(DisplayName = "Map"),
	EquipMagic		UMETA(DisplayName = "EquipMagic"),
	UpgradeStatus	UMETA(DisplayName = "Status_Upgrade"),
};

class UButton;

UCLASS()
class ADVANCEDCOMBAT_API UWidget_GameMenu_Button : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameMenuCategory MenuCategory;

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UButton* Button_Menu;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void MenuButtonClicked();

};
