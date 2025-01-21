// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/GameMenu/Widget_GameMenu_Button.h"
#include "Components/Button.h"
#include "Subsystem/UISubsystem.h"
#include "ACGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/Equipment/Widget_EquipSelectWindow.h"
#include "Component/InventoryComponent.h"

void UWidget_GameMenu_Button::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Menu->OnClicked.AddUniqueDynamic(this, &UWidget_GameMenu_Button::MenuButtonClicked);
}

void UWidget_GameMenu_Button::MenuButtonClicked()
{
	UUISubsystem* UIManager = GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UIManager) return;

	switch (MenuCategory)
	{
	case EGameMenuCategory::Inventory:
	{
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_GameMenu, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Inventory, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::Equipment:
	{
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_GameMenu, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Equipment, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::Setting:
	{
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_GameMenu, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Setting, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::EquipMagic:
	{
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_EquipMagic, ESlateVisibility::SelfHitTestInvisible);

		UUserWidget* widget = UIManager->GetWidget(FACGameplayTags::Get().UI_Widget_GameMenu_EquipMagic);
		if (!widget) return;
		UWidget_EquipSelectWindow* userWidget = Cast<UWidget_EquipSelectWindow>(widget);
		if (!userWidget) return;
		userWidget->UpdateContents(EItemCategory::Magic);

		break;
	}
	case EGameMenuCategory::Map:
	{
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Map, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::UpgradeStatus:
	{
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Santuary, ESlateVisibility::Collapsed);
		UIManager->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_UpgradeStatus, ESlateVisibility::SelfHitTestInvisible);
		break;
	}
	case EGameMenuCategory::Quit:
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Type::Quit, false);
		break;
	}
	default:
		break;
	}
}

