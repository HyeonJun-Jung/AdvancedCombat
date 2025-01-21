// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ACPlayerController.h"
#include "Component/InventoryComponent.h"
#include "Component/SlotComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/UISubsystem.h"
#include "ACGameplayTags.h"

AACPlayerController::AACPlayerController()
{
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	slotComp = CreateDefaultSubobject<USlotComponent>("SlotComp");
}

void AACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass_HUD)
	{
		UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, WidgetClass_HUD);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AACPlayerController::Interact);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AACPlayerController::ShowInventory);
	
		// GameMenu
		EnhancedInputComponent->BindAction(GameMenuAction, ETriggerEvent::Started, this, &AACPlayerController::ShowGameMenu);
	}
}

void AACPlayerController::Interact()
{
	if (InventoryComp)
	{
		InventoryComp->Interact();
	}
}

void AACPlayerController::ShowInventory()
{
	UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	UISystem->SetLayerVisibility(FACGameplayTags::Get().UI_Layer_GameMenu,
		ESlateVisibility::SelfHitTestInvisible);
	UISystem->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_Inventory, 
		ESlateVisibility::SelfHitTestInvisible);
	EnableUIInteract();

	Delegate_ShowInventory.Broadcast();
}

void AACPlayerController::ShowGameMenu()
{
	UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	UISystem->SetLayerVisibility(FACGameplayTags::Get().UI_Layer_GameMenu,
		ESlateVisibility::SelfHitTestInvisible);
	UISystem->SetWidgetVisibility(FACGameplayTags::Get().UI_Widget_GameMenu_GameMenu,
		ESlateVisibility::SelfHitTestInvisible);
	EnableUIInteract();
}

void AACPlayerController::EnableUIInteract()
{
	FInputModeUIOnly inputMode;
	SetInputMode(inputMode);
	SetShowMouseCursor(true);
}

void AACPlayerController::DisableUIInteract()
{
	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
	SetShowMouseCursor(false);
}
