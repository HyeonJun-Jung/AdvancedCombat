// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ACEnums.h"
#include "ACStructs.generated.h"


USTRUCT(BlueprintType)
struct ADVANCEDCOMBAT_API FACStructs
{
	GENERATED_BODY()
	FACStructs() {}
};

USTRUCT(BlueprintType)
struct ADVANCEDCOMBAT_API FItemStruct : public FTableRowBase
{
	GENERATED_BODY()
	FItemStruct()
	{
		EquipType = EEquipCategory::EEC_None;
		WeaponType = EWeaponType::EWT_None;
	}

	// Item Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EItemCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	uint8 MaxStackSize;

	// // // // // // // // // // // // //
	// // // For Equipment System // // // 
	// // // // // // // // // // // // //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	EEquipCategory EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	USkeletalMesh* SkeletonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	USkeletalMesh* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	TSubclassOf<UAnimInstance> WeaponAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	FTransform RelativeTransform;
};

USTRUCT(BlueprintType)
struct ADVANCEDCOMBAT_API FEquipmentStatStruct : public FTableRowBase
{
	GENERATED_BODY()
	FEquipmentStatStruct() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ATK_Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ATK_Max = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DEF_Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DEF_Max = 0;
};

USTRUCT(BlueprintType)
struct ADVANCEDCOMBAT_API FSlotStruct
{
	GENERATED_BODY()

	FSlotStruct() {}

	FSlotStruct& operator=(const FSlotStruct& InSlot)
	{
		ID = InSlot.ID; Category = InSlot.Category; ItemName = InSlot.ItemName;
		Quantity = InSlot.Quantity; EquipStatus = InSlot.EquipStatus;
		return *this;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Quantity;

	// For Equipment 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEquipmentStatStruct EquipStatus;
};
