// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACPlayerController.generated.h"

class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowInventory);

UCLASS()
class ADVANCEDCOMBAT_API AACPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** GameMenu Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GameMenuAction;

	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	/** Inventory Widget Class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget | HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WidgetClass_HUD;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USlotComponent* slotComp;

public:
	AACPlayerController();

public:
	virtual void BeginPlay() override;

protected:
	virtual void SetupInputComponent() override;

public:
	class USlotComponent* GetSlotComponent() { return slotComp; }

public:
	void Interact();
	void ShowInventory();
	void ShowGameMenu();

public:
	UFUNCTION(BlueprintCallable)
	void EnableUIInteract();

	UFUNCTION(BlueprintCallable)
	void DisableUIInteract();

public:
	UPROPERTY(BlueprintAssignable)
	FShowInventory Delegate_ShowInventory;
};
