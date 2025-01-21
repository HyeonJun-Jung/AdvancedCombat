// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACStructs.h"
#include "ACEnums.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FInventoryUpdated)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDCOMBAT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Interact Setting
public:
	UPROPERTY(EditAnywhere)
	float Interact_Range = 500;

	UPROPERTY(EditAnywhere)
	float Interact_Radius = 50;

private:
	class IInteract_Interface* Interacting_Actor = nullptr;

	// Inventory Delegate
public:
	FInventoryUpdated Delegate_InventoryUpdated;

private:
	UPROPERTY()
	UDataTable* ItemDB;

	UPROPERTY(EditAnywhere)
	int Inventory_Size = 64;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	uint8 CurrentQuickSlotNum = 0;

	TArray<FSlotStruct> QuickSlots;

	TArray<FSlotStruct> None;

	TArray<FSlotStruct> Weapons;

	TArray<FSlotStruct> Runes;

	TArray<FSlotStruct> FragmentOfGods;

	TArray<FSlotStruct> UseableItems;

	TArray<FSlotStruct> Upgrades;

	TArray<FSlotStruct> Huntings;

	TArray<FACMagicStruct> Magics;

public:
	void Interact();

public:

	FName GetFNameOfItemID(int ItemID);
	UDataTable* GetItemDB() { return ItemDB; };
	TArray<FSlotStruct>& GetContents(EItemCategory InCategory);
	TArray<FACMagicStruct>& GetMagics();
	TArray<FSlotStruct>& GetQuickSlots() { return QuickSlots; }
	void GetAllSlotsOfCategory(TArray<FSlotStruct>& InSlotArray, EItemCategory InItemCategory, EEquipCategory InEquipCategory);
	void LogInventoryContents(EItemCategory InCategory);

public:
	UFUNCTION(BlueprintCallable)
	void AddToInventory(FSlotStruct ItemSlot);

	UFUNCTION(BlueprintCallable)
	bool AddToInventory_Slot(const FSlotStruct& ItemSlot, int slotIdx);

	UFUNCTION(BlueprintCallable)
	void RemoveItem_QuickSlot(int SlotIndex, bool Consumed, int Quantity);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(EItemCategory InCategory, int SlotIndex, bool Consumed, int Quantity);

	void RemoveItem(EItemCategory InCategory, int ItemID, int Quantity);

	UFUNCTION(BlueprintCallable)
	void ReplaceItem(int SlotIndex, FSlotStruct& ReplaceSlotData, int Quantity);

	int GetItemQuantityFromID(EItemCategory InCategory, int itemID);
	int GetItemQuantityFromSlot(EItemCategory InCategory, int slotIdx);

private:
	void InitInventory();
	void InitMagic();
};
