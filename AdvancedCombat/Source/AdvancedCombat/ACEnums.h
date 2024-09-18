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