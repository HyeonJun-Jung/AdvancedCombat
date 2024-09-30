// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetTextLibrary.h>
#include "Interface/Interact_Interface.h"
#include "Actor/Item_Base.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UDataTable* BP_ItemDB = LoadObject<UDataTable>(this, TEXT("/Game/AdvancedCombat/Inventory/Data/DB_Item.DB_Item"));
	if (IsValid(BP_ItemDB))
	{
		ItemDB = BP_ItemDB;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't Get Item DataTable."))
	}

	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		Contents.Add(item);
	}

	for (int i = 0; i < 8; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		QuickSlots.Add(item);
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::Interact()
{
	APawn* pawn = Cast<APawn>(GetOwner());
	if (!pawn) return;

	AController* controller = pawn->GetController();
	if (!controller) return;

	TArray<AActor*> IgnoreActors;	FHitResult hitResult;
	IgnoreActors.Add(pawn);

	FVector start = pawn->GetActorLocation();
	FRotator ControllerRot = controller->GetControlRotation();
	FVector ControllerForwardVec = UKismetMathLibrary::GetForwardVector(ControllerRot);
	// FVector end = start + Interact_Range * owner->GetActorForwardVector();
	FVector end = start + Interact_Range * ControllerForwardVec;

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

	UKismetSystemLibrary::SphereTraceSingle(pawn, start, end, Interact_Radius,
		TraceType, false, IgnoreActors,
		EDrawDebugTrace::ForOneFrame, hitResult, true);

	IInteract_Interface* hitActor = Cast<IInteract_Interface>(hitResult.GetActor());
	AItem_Base* hitItem = Cast<AItem_Base>(hitActor);
	if (hitActor)
	{
		hitActor->Execute_Interact_With(hitItem, this);
		/*AInteractable_Actor* interactable_Actor = Cast<AInteractable_Actor>(Interacting_Actor);
		if (IsValid(interactable_Actor))
		{
			interactable_Actor->Execute_Interact_With(interactable_Actor, this);
			interactable_Actor->Show_InteractUI(false);
			return;
		}*/
	}
}

FName UInventoryComponent::GetFNameOfItemID(int ItemID)
{
	return  FName(*FString::FromInt(ItemID));
}

void UInventoryComponent::LogInventoryContents()
{
	for (FSlotStruct slot : Contents)
	{
		if (slot.ID != -1)
			UE_LOG(LogTemp, Warning, TEXT("ID : %s, Quantity : %d"), *slot.ItemName.ToString(), slot.Quantity);
	}
}


void UInventoryComponent::AddToInventory(FSlotStruct ItemSlot)
{
	if (!ItemDB)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't Get DataTable."));
		return;
	}

	FItemStruct* itemData = ItemDB->FindRow<FItemStruct>(GetFNameOfItemID(ItemSlot.ID), ItemSlot.ItemName.ToString());
	if (!itemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : ID <%s> Can't Add To Inventory."), *ItemSlot.ItemName.ToString());
		return;
	}

	for (FSlotStruct& slot : Contents)
	{
		if (slot.ID == ItemSlot.ID && slot.Category != EItemCategory::EIC_Equipment
			&& slot.Quantity + ItemSlot.Quantity <= itemData->MaxStackSize)
		{
			slot.Quantity += ItemSlot.Quantity;

			//auto controller = Cast<ADPPlayerController>(GetOwner());
			//if (IsValid(controller))
			//{
			//	Delegate_InventoryUpdated.Broadcast();
			//	// controller->UpdateInventory();
			//	controller->UpdateAcquire(itemData->Name, ItemSlot.Quantity);
			//	controller->UpdateQuest_Item();
			//}

			return;
		}
	}

	for (FSlotStruct& slot : Contents)
	{
		if (slot.Quantity == 0 || slot.ID == -1)
		{
			slot = ItemSlot;

			//auto controller = Cast<ADPPlayerController>(GetOwner());
			//if (IsValid(controller))
			//{
			//	Delegate_InventoryUpdated.Broadcast();
			//	// controller->UpdateInventory();
			//	controller->UpdateAcquire(itemData->Name, ItemSlot.Quantity);
			//	controller->UpdateQuest_Item();
			//}

			return;
		}
	}
}

bool UInventoryComponent::AddToInventory_Slot(const FSlotStruct& ItemSlot, int slotIdx)
{
	if (Contents[slotIdx].ID == ItemSlot.ID)
	{
		Contents[slotIdx].Quantity += ItemSlot.Quantity;
		Delegate_InventoryUpdated.Broadcast();
		return true;
	}

	if (Contents[slotIdx].Quantity == 0)
	{
		Contents[slotIdx] = ItemSlot;
		Delegate_InventoryUpdated.Broadcast();
		return true;
	}

	return false;
}

void UInventoryComponent::RemoveItem_QuickSlot(int SlotIndex, bool Consumed, int Quantity)
{
}

void UInventoryComponent::RemoveItem(int SlotIndex, bool Consumed, int Quantity)
{
	if (Contents[SlotIndex].Quantity <= 0)
		return;

	if (Consumed)
	{
		Contents[SlotIndex].Quantity -= Quantity;
	}
	else
	{
		FItemStruct* itemData = ItemDB->FindRow<FItemStruct>(GetFNameOfItemID(Contents[SlotIndex].ID), Contents[SlotIndex].ItemName.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't find Item ID <%s> From Contents."), *Contents[SlotIndex].ItemName.ToString());
			return;
		}

		Contents[SlotIndex].Quantity -= Quantity;
	}
}

void UInventoryComponent::RemoveItem(int ItemID, int Quantity)
{
	int RemovedQuantity = 0;
	for (auto& slot : Contents)
	{
		if (slot.ID == ItemID)
		{
			if (RemovedQuantity + slot.Quantity >= Quantity)
			{
				int addQuantity = Quantity - RemovedQuantity;
				RemovedQuantity += addQuantity;
				slot.Quantity -= addQuantity;
				break;
			}
			else
			{
				RemovedQuantity += slot.Quantity;
				slot.Quantity = 0;
			}
		}
	}

	Delegate_InventoryUpdated.Broadcast();
}

void UInventoryComponent::ReplaceItem(int SlotIndex, FSlotStruct& ReplaceSlotData, int Quantity)
{
	Contents[SlotIndex] = ReplaceSlotData;
}

void UInventoryComponent::Transfer_Slot(UInventoryComponent* SourceInv, int SourceIdx, int DestIdx)
{
	TArray<FSlotStruct>& SourceContents = SourceInv->GetContents();
	TArray<FSlotStruct>& DestContents = GetContents();

	// Case : SourceItemID == DestItemID
	if (DestContents[DestIdx].ID == SourceContents[SourceIdx].ID)
	{
		if (SourceIdx == DestIdx && SourceInv == this)
			return;

		UE_LOG(LogTemp, Warning, TEXT("Local Contests[%d] : %s"), DestIdx, *DestContents[DestIdx].ItemName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Source Contests[%d] : %s"), SourceIdx, *SourceContents[DestIdx].ItemName.ToString());
		FItemStruct* itemData = ItemDB->FindRow<FItemStruct>(GetFNameOfItemID(DestContents[DestIdx].ID), DestContents[DestIdx].ItemName.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't find Item ID <%s> From Contents."), *DestContents[DestIdx].ItemName.ToString());
			return;
		}

		// Case : Dest ItemQuantity + Source ItemQuantity <= MaxStackSize
		if (DestContents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity <= itemData->MaxStackSize)
		{
			DestContents[DestIdx].Quantity += SourceContents[SourceIdx].Quantity;
			SourceContents[SourceIdx].Quantity = 0;
			SourceContents[SourceIdx].ID = -1;
		}
		// Case : Dest ItemQuantity + Source ItemQuantity > MaxStackSize
		else
		{
			int TransferQuantity
				= DestContents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity - itemData->MaxStackSize;

			DestContents[DestIdx].Quantity += TransferQuantity;
			SourceContents[SourceIdx].Quantity -= TransferQuantity;
		}
	}
	// Case : SourceItemID != DestItemID
	else
	{
		FSlotStruct LocalSlot = DestContents[DestIdx];
		FSlotStruct SrcSlot = SourceContents[SourceIdx];
		DestContents[DestIdx] = SrcSlot;
		SourceContents[SourceIdx] = LocalSlot;
	}

	/*auto controller = Cast<ADPPlayerController>(GetOwner());
	if (IsValid(controller))
	{
		Delegate_InventoryUpdated.Broadcast();
	}*/
}

void UInventoryComponent::Transfer_Slot(UInventoryComponent* SourceInv, int SourceIdx, UInventoryComponent* DestInv, int DestIdx)
{
	TArray<FSlotStruct>& SourceContents = SourceInv->GetContents();
	TArray<FSlotStruct>& DestContents = DestInv->GetContents();

	// Case : SourceItemID == DestItemID
	if (DestContents[DestIdx].ID == SourceContents[SourceIdx].ID)
	{
		if (SourceIdx == DestIdx && SourceInv == DestInv)
			return;

		UE_LOG(LogTemp, Warning, TEXT("Local Contests[%d] : %s"), DestIdx, *DestContents[DestIdx].ItemName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Source Contests[%d] : %s"), SourceIdx, *SourceContents[DestIdx].ItemName.ToString());
		FItemStruct* itemData = ItemDB->FindRow<FItemStruct>(GetFNameOfItemID(DestContents[DestIdx].ID), DestContents[DestIdx].ItemName.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't find Item ID <%s> From Contents."), *DestContents[DestIdx].ItemName.ToString());
			return;
		}

		// Case : Dest ItemQuantity + Source ItemQuantity <= MaxStackSize
		if (DestContents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity <= itemData->MaxStackSize)
		{
			DestContents[DestIdx].Quantity += SourceContents[SourceIdx].Quantity;
			SourceContents[SourceIdx].Quantity = 0;
			SourceContents[SourceIdx].ID = -1;
		}
		// Case : Dest ItemQuantity + Source ItemQuantity > MaxStackSize
		else
		{
			int TransferQuantity
				= DestContents[DestIdx].Quantity + SourceContents[SourceIdx].Quantity - itemData->MaxStackSize;

			DestContents[DestIdx].Quantity += TransferQuantity;
			SourceContents[SourceIdx].Quantity -= TransferQuantity;
		}
	}
	// Case : SourceItemID != DestItemID
	else
	{
		FSlotStruct LocalSlot = DestContents[DestIdx];
		FSlotStruct SrcSlot = SourceContents[SourceIdx];
		DestContents[DestIdx] = SrcSlot;
		SourceContents[SourceIdx] = LocalSlot;
	}

	Delegate_InventoryUpdated.Broadcast();
}

void UInventoryComponent::Transfer_Slot_InvToQuick(int InventoryIdx, int QuickSlotIdx)
{
	TArray<FSlotStruct>& Inventory = GetContents();
	TArray<FSlotStruct>& QuickSlot = GetQuickSlots();

	// Case : SourceItemID == DestItemID
	if (Inventory[InventoryIdx].ID == QuickSlot[QuickSlotIdx].ID)
	{
		FItemStruct* itemData = ItemDB->FindRow<FItemStruct>(GetFNameOfItemID( Inventory[InventoryIdx].ID), Inventory[InventoryIdx].ItemName.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Can't find Item ID <%s> From Contents."), *Inventory[InventoryIdx].ItemName.ToString());
			return;
		}

		// Case : Dest ItemQuantity + Source ItemQuantity <= MaxStackSize
		if (QuickSlot[QuickSlotIdx].Quantity + Inventory[InventoryIdx].Quantity <= itemData->MaxStackSize)
		{
			QuickSlot[QuickSlotIdx].Quantity += Inventory[InventoryIdx].Quantity;
			Inventory[InventoryIdx].Quantity = 0;
			Inventory[InventoryIdx].ID = -1;
		}
		// Case : Dest ItemQuantity + Source ItemQuantity > MaxStackSize
		else
		{
			int TransferQuantity
				= QuickSlot[QuickSlotIdx].Quantity + Inventory[InventoryIdx].Quantity - itemData->MaxStackSize;

			QuickSlot[QuickSlotIdx].Quantity += TransferQuantity;
			Inventory[InventoryIdx].Quantity -= TransferQuantity;
		}
	}
	else
	{
		FSlotStruct InvLocalSlot = Inventory[InventoryIdx];
		FSlotStruct QuickLocalSlot = QuickSlot[QuickSlotIdx];
		Inventory[InventoryIdx] = QuickLocalSlot;
		QuickSlot[QuickSlotIdx] = InvLocalSlot;
	}


	Delegate_InventoryUpdated.Broadcast();
}

void UInventoryComponent::Transfer_Slot_QuickToInv(int InventoryIdx, int QuickSlotIdx)
{
	TArray<FSlotStruct>& Inventory = GetContents();
	TArray<FSlotStruct>& QuickSlot = GetQuickSlots();

	// Case : SourceItemID == DestItemID
	if (Inventory[InventoryIdx].ID == QuickSlot[QuickSlotIdx].ID)
	{
		FItemStruct* itemData = ItemDB->FindRow<FItemStruct>(GetFNameOfItemID(Inventory[InventoryIdx].ID), Inventory[InventoryIdx].ItemName.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("APlayer_Controller : Can't find Item ID <%s> From Contents."), *Inventory[InventoryIdx].ItemName.ToString());
			return;
		}

		// Case : Dest ItemQuantity + Source ItemQuantity <= MaxStackSize
		if (QuickSlot[QuickSlotIdx].Quantity + Inventory[InventoryIdx].Quantity <= itemData->MaxStackSize)
		{
			Inventory[QuickSlotIdx].Quantity += QuickSlot[InventoryIdx].Quantity;
			QuickSlot[InventoryIdx].Quantity = 0;
			QuickSlot[InventoryIdx].ID = -1;
		}
		// Case : Dest ItemQuantity + Source ItemQuantity > MaxStackSize
		else
		{
			int TransferQuantity
				= QuickSlot[QuickSlotIdx].Quantity + Inventory[InventoryIdx].Quantity - itemData->MaxStackSize;

			Inventory[QuickSlotIdx].Quantity += TransferQuantity;
			QuickSlot[InventoryIdx].Quantity -= TransferQuantity;
		}
	}
	else
	{
		FSlotStruct InvLocalSlot = Inventory[InventoryIdx];
		FSlotStruct QuickLocalSlot = QuickSlot[QuickSlotIdx];
		Inventory[InventoryIdx] = QuickLocalSlot;
		QuickSlot[QuickSlotIdx] = InvLocalSlot;
	}

	Delegate_InventoryUpdated.Broadcast();
}

void UInventoryComponent::Transfer_Slot_QuickToQuick(int SourceIdx, int DestIdx)
{
	if (SourceIdx == DestIdx)
		return;

	TArray<FSlotStruct>& QuickSlot = GetQuickSlots();

	// Case : SourceItemID == DestItemID
	if (QuickSlot[SourceIdx].ID == QuickSlot[DestIdx].ID)
	{
		FItemStruct* itemData = ItemDB->FindRow<FItemStruct>(GetFNameOfItemID( QuickSlot[SourceIdx].ID), QuickSlot[SourceIdx].ItemName.ToString());
		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("APlayer_Controller : Can't find Item ID <%s> From Contents."), *QuickSlot[SourceIdx].ItemName.ToString());
			return;
		}

		// Case : Dest ItemQuantity + Source ItemQuantity <= MaxStackSize
		if (QuickSlot[SourceIdx].Quantity + QuickSlot[DestIdx].Quantity <= itemData->MaxStackSize)
		{
			QuickSlot[DestIdx].Quantity += QuickSlot[SourceIdx].Quantity;
			QuickSlot[SourceIdx].Quantity = 0;
			QuickSlot[SourceIdx].ID = -1;
		}
		// Case : Dest ItemQuantity + Source ItemQuantity > MaxStackSize
		else
		{
			int TransferQuantity
				= QuickSlot[SourceIdx].Quantity + QuickSlot[DestIdx].Quantity - itemData->MaxStackSize;

			QuickSlot[DestIdx].Quantity += TransferQuantity;
			QuickSlot[SourceIdx].Quantity -= TransferQuantity;
		}
	}
	else
	{
		FSlotStruct SourceLocalSlot = QuickSlot[SourceIdx];
		FSlotStruct DestLocalSlot = QuickSlot[DestIdx];
		QuickSlot[SourceIdx] = DestLocalSlot;
		QuickSlot[DestIdx] = SourceLocalSlot;
	}

	Delegate_InventoryUpdated.Broadcast();
}

int UInventoryComponent::GetItemQuantityFromID(int itemID)
{
	int Quantity = 0;
	for (auto& slot : Contents)
	{
		if (slot.ID == itemID)
		{
			Quantity += slot.Quantity;
		}
	}
	return Quantity;
}

int UInventoryComponent::GetItemQuantityFromSlot(int slotIdx)
{
	if (Contents.IsValidIndex(slotIdx))
		return Contents[slotIdx].Quantity;
	return 0;
}