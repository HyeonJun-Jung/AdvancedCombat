// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ACStructs.h"
#include "ACEnums.h"
#include "InventorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UInventorySubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FItemStruct* GetItemInfo(int itemID);
	FWeaponStruct* GetWeaponInfo(int itemID);
	const FACMagicStruct* GetMagicInfo(int magicID);

	const TMap<int, FACMagicStruct>& GetMagicMap() { return MagicMap; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FItemStruct> ItemMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FWeaponStruct> WeaponMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FACMagicStruct> MagicMap;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> ItemDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> WeaponDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> MagicDB;

};
