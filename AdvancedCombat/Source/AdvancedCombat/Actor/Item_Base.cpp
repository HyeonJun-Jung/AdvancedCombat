// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item_Base.h"
#include "ACStructs.h"
#include "Component/InventoryComponent.h"

// Sets default values
AItem_Base::AItem_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem_Base::Interact_With_Implementation(UInventoryComponent* InventoryComponent)
{
	if (ItemDataRow.DataTable)
	{
		FItemStruct* itemData = 
			ItemDataRow.DataTable->FindRow<FItemStruct>(ItemDataRow.RowName, ItemDataRow.RowName.ToString());
		
		if (itemData)
		{
			FSlotStruct slot = FSlotStruct();
			slot.ID = itemData->ID; slot.ItemName = itemData->ItemName; slot.Category = itemData->Category;
			slot.Quantity = 1;
			InventoryComponent->AddToInventory(slot);
		}
	}
}

