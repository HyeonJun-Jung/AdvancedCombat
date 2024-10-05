// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ACPlayerController.h"
#include "Component/InventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Widget/InGame/Inventory/Widget_Inventory_Content.h"

AACPlayerController::AACPlayerController()
{
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("Inventory");
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
	Delegate_ShowInventory.Broadcast();
}
