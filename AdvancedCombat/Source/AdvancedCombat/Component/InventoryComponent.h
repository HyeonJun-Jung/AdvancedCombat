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

	// Interact
public:
	UPROPERTY(EditAnywhere)
	float Interact_Range = 500;

	UPROPERTY(EditAnywhere)
	float Interact_Radius = 50;

	FInventoryUpdated Delegate_InventoryUpdated;

private:
	class IInteract_Interface* Interacting_Actor = nullptr;


private:
	UPROPERTY()
	UDataTable* ItemDB;

	UPROPERTY(EditAnywhere)
	int Inventory_Size = 64;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	uint8 CurrentQuickSlotNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<FSlotStruct> QuickSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<FSlotStruct> Contents;

public:
	void Interact();

public:
	FName GetFNameOfItemID(int ItemID);
	UDataTable* GetItemDB() { return ItemDB; };
	TArray<FSlotStruct>& GetContents() { return Contents; }
	TArray<FSlotStruct>& GetQuickSlots() { return QuickSlots; }
	void LogInventoryContents();

	UFUNCTION(BlueprintCallable)
	void AddToInventory(FSlotStruct ItemSlot);

	UFUNCTION(BlueprintCallable)
	bool AddToInventory_Slot(const FSlotStruct& ItemSlot, int slotIdx);

	UFUNCTION(BlueprintCallable)
	void RemoveItem_QuickSlot(int SlotIndex, bool Consumed, int Quantity);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(int SlotIndex, bool Consumed, int Quantity);

	void RemoveItem(int ItemID, int Quantity);

	UFUNCTION(BlueprintCallable)
	void ReplaceItem(int SlotIndex, FSlotStruct& ReplaceSlotData, int Quantity);

	UFUNCTION(BlueprintCallable)
	void Transfer_Slot(UInventoryComponent* SourceInv, int SourceIdx, int DestIdx);
	void Transfer_Slot(UInventoryComponent* SourceInv, int SourceIdx, UInventoryComponent* DestInv, int DestIdx);
	void Transfer_Slot_InvToQuick(int InventoryIdx, int QuickSlotIdx);
	void Transfer_Slot_QuickToInv(int InventoryIdx, int QuickSlotIdx);
	void Transfer_Slot_QuickToQuick(int SourceIdx, int DestIdx);

	int GetItemQuantityFromID(int itemID);
	int GetItemQuantityFromSlot(int slotIdx);
};
