// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetTextLibrary.h>
#include "Interface/Interact_Interface.h"
#include "Actor/Item_Base.h"
#include "Subsystem/InventorySubsystem.h"

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

	InitInventory();
	InitMagic();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::Interact()
{
	AController* controller = Cast<AController>(GetOwner());
	if (!controller) return;

	APawn* pawn = controller->GetPawn();
	if (!pawn) return;

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

TArray<FSlotStruct>& UInventoryComponent::GetContents(EItemCategory InCategory)
{
	switch (InCategory)
	{
	case EItemCategory::None:
		break;
	case EItemCategory::Weapon:
		return Weapons;
		break;
	case EItemCategory::Rune:
		return Runes;
		break;
	case EItemCategory::FragmentOfGod:
		return FragmentOfGods;
		break;
	case EItemCategory::UseableItem:
		return UseableItems;
		break;
	case EItemCategory::Magic:
		return None;
		break;
	case EItemCategory::Upgrades:
		return Upgrades;
		break;
	case EItemCategory::Hunting:
		return Huntings;
		break;
	default:
		break;
	}

	return None;
}

TArray<FACMagicStruct>& UInventoryComponent::GetMagics()
{
	return Magics;
}

void UInventoryComponent::GetAllSlotsOfCategory(TArray<FSlotStruct>& InSlotArray, EItemCategory InItemCategory, EEquipCategory InEquipCategory)
{
	TArray<FSlotStruct>& Contents = GetContents(InItemCategory);
	for (FSlotStruct& slot : Contents)
	{
		if (slot.Category == InItemCategory)
		{
			InSlotArray.Add(slot);
		}
	}
}

void UInventoryComponent::LogInventoryContents(EItemCategory InCategory)
{
	TArray<FSlotStruct>& Contents = GetContents(InCategory);
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

	TArray<FSlotStruct>& Contents = GetContents(ItemSlot.Category);
	for (FSlotStruct& slot : Contents)
	{
		if (slot.ID == ItemSlot.ID && slot.Quantity + ItemSlot.Quantity <= itemData->MaxStackSize)
		{
			slot.Quantity += ItemSlot.Quantity;
			Delegate_InventoryUpdated.Broadcast();

			return;
		}
	}

	for (FSlotStruct& slot : Contents)
	{
		if (slot.Quantity == 0 || slot.ID == -1)
		{
			slot = ItemSlot;
			Delegate_InventoryUpdated.Broadcast();

			return;
		}
	}
}

bool UInventoryComponent::AddToInventory_Slot(const FSlotStruct& ItemSlot, int slotIdx)
{
	TArray<FSlotStruct>& Contents = GetContents(ItemSlot.Category);
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

void UInventoryComponent::RemoveItem(EItemCategory InCategory, int SlotIndex, bool Consumed, int Quantity)
{
	TArray<FSlotStruct>& Contents = GetContents(InCategory);

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

void UInventoryComponent::RemoveItem(EItemCategory InCategory, int ItemID, int Quantity)
{
	TArray<FSlotStruct>& Contents = GetContents(InCategory);

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
	TArray<FSlotStruct>& Contents = GetContents(ReplaceSlotData.Category);
	Contents[SlotIndex] = ReplaceSlotData;
}

int UInventoryComponent::GetItemQuantityFromID(EItemCategory InCategory, int itemID)
{
	int Quantity = 0;
	TArray<FSlotStruct>& Contents = GetContents(InCategory);
	for (auto& slot : Contents)
	{
		if (slot.ID == itemID)
		{
			Quantity += slot.Quantity;
		}
	}
	return Quantity;
}

int UInventoryComponent::GetItemQuantityFromSlot(EItemCategory InCategory, int slotIdx)
{
	TArray<FSlotStruct>& Contents = GetContents(InCategory);
	if (Contents.IsValidIndex(slotIdx))
		return Contents[slotIdx].Quantity;
	return 0;
}

void UInventoryComponent::InitInventory()
{
	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		Weapons.Add(item);
	}

	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		Runes.Add(item);
	}

	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		FragmentOfGods.Add(item);
	}

	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		UseableItems.Add(item);
	}

	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		Upgrades.Add(item);
	}

	for (int i = 0; i < Inventory_Size; i++)
	{
		FSlotStruct item; item.ID = -1; item.Quantity = 0;
		Huntings.Add(item);
	}
}

void UInventoryComponent::InitMagic()
{
	UInventorySubsystem* invSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();

	const TMap<int, FACMagicStruct>& magicMap = invSystem->GetMagicMap();
	for (auto& magic : magicMap)
	{
		Magics.Add(magic.Value);
	}
}
