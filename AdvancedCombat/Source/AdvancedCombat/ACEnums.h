// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETaskResult : uint8
{
	// Task is executed Successly
	ETR_Success,
	// Parry Detected while Task
	ETR_Parried,
	// Take Damage while Task
	ETR_Damaged,
};

UENUM(BlueprintType)
enum class EACAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	Confirm			UMETA(DisplayName = "Confirm"),
	Cancel			UMETA(DisplayName = "Cancel"),
	Jump			UMETA(DisplayName = "Jump"),
	Attack			UMETA(DisplayName = "Attack"),
	Aiming			UMETA(DisplayName = "Aiming"),
	Interact		UMETA(DisplayName = "Interact"),
	Tab				UMETA(DisplayName = "Tab"),
	Ability01		UMETA(DisplayName = "Ability01"),
	Ability02		UMETA(DisplayName = "Ability02"),
	Ability03		UMETA(DisplayName = "Ability03"),
	Ability04		UMETA(DisplayName = "Ability04"),
};

UENUM(BlueprintType)
enum class EACHitReactDirection : uint8
{
	// 0
	None			UMETA(DisplayName = "None"),
	// 1
	Left 			UMETA(DisplayName = "Left"),
	// 2
	Front 			UMETA(DisplayName = "Front"),
	// 3
	Right			UMETA(DisplayName = "Right"),
	// 4
	Back			UMETA(DisplayName = "Back")
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	NormalDamage,
	GuardableDamage,
	ParryableDamage,
	UnbreakableDamage,
	StingDamage,
};

UENUM(BlueprintType)
enum class EParryType : uint8
{
	LeftToRight,
	RightToLeft
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	EIC_Item,
	EIC_Equipment,
	EIC_None
};

UENUM(BlueprintType)
enum class EEquipCategory : uint8
{
	EEC_None,
	EEC_Weapon,
	EEC_Hat,
	EEC_Chest,
	EEC_Pants,
	EEC_Shoes,
	EEC_Cave,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None,
	EWT_Katana,
	EWT_GreatSword,
	EWT_SwordShield,
	EWT_DualBlade,
	EWT_Spear,
	EWT_Bow,
};