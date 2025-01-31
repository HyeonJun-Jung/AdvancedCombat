// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	Up,
	Down,
	Left,
	Right
};

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
	Run				UMETA(DisplayName = "Run"),
	LockOn			UMETA(DisplayName = "LockOn"),
	Attack			UMETA(DisplayName = "Attack"),
	Defense			UMETA(DisplayName = "Defense"),
	Aiming			UMETA(DisplayName = "Aiming"),
	Interact		UMETA(DisplayName = "Interact"),
	Tab				UMETA(DisplayName = "Tab"),
	Signature		UMETA(DisplayName = "Signature"),
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
	AirborneDamage,
	InAirDamage,
	HitDownDamage
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
	None,
	Weapon,
	Rune,			// defense or specific effect
	FragmentOfGod,	// Special skill
	UseableItem,	// Useable Item
	Magic,
	Upgrades,		// weapon & potion upgrade Items
	Hunting			// monster drop item
};

UENUM(BlueprintType)
enum class EEquipCategory : uint8
{
	EEC_None,
	EEC_Weapon,
	EEC_Helmet,
	EEC_Chest,
	EEC_Pants,
	EEC_Glove,
	EEC_Ring,
	EEC_Cave,
	EEC_Usable,
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

UENUM(BlueprintType)
enum class EUpgradeItemType : uint8
{
	Sword,
	Spear,
	Staff,
	Potion
};