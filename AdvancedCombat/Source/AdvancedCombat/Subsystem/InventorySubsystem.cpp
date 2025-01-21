// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/InventorySubsystem.h"

UInventorySubsystem::UInventorySubsystem()
{
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ItemDB = LoadObject<UDataTable>(this, TEXT("/Script/Engine.CompositeDataTable'/Game/AdvancedCombat/Inventory/Data/DB_Item.DB_Item'"));
	if (ItemDB == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySubsystem : Failed to Load ItemDB !!! "));
	}

	TArray<FName> rowNames = ItemDB->GetRowNames();
	for (FName RowName : rowNames)
	{
		FItemStruct* ItemInfo = ItemDB->FindRow<FItemStruct>(RowName, TEXT("FindRow"));
		if (ItemInfo != nullptr)
		{
			ItemMap.Add(ItemInfo->ID, *ItemInfo);
		}
	}

	WeaponDB = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/AdvancedCombat/Inventory/Data/Weapon/DB_WeaponStat.DB_WeaponStat'"));
	if (WeaponDB == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySubsystem : Failed to Load WeaponDB !!! "));
	}

	rowNames = WeaponDB->GetRowNames();
	for (FName RowName : rowNames)
	{
		FWeaponStruct* weaponInfo = WeaponDB->FindRow<FWeaponStruct>(RowName, TEXT("FindRow"));
		if (weaponInfo != nullptr)
		{
			WeaponMap.Add(weaponInfo->ID, *weaponInfo);
		}
	}

	MagicDB = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/AdvancedCombat/Characters/Player/Abilities/Magic/DB/DB_Magic.DB_Magic'"));
	if (MagicDB == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySubsystem : Failed to Load MagicDB !!! "));
	}

	rowNames = MagicDB->GetRowNames();
	for (FName RowName : rowNames)
	{
		FACMagicStruct* magicInfo = MagicDB->FindRow<FACMagicStruct>(RowName, TEXT("FindRow"));
		if (magicInfo != nullptr)
		{
			MagicMap.Add(magicInfo->MagicID, *magicInfo);
		}
	}
}

const FItemStruct* UInventorySubsystem::GetItemInfo(int itemID)
{
	return ItemMap.Find(itemID);
}

FWeaponStruct* UInventorySubsystem::GetWeaponInfo(int itemID)
{
	return WeaponMap.Find(itemID);
}

const FACMagicStruct* UInventorySubsystem::GetMagicInfo(int magicID)
{
	return MagicMap.Find(magicID);
}
