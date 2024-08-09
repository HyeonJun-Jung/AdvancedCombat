// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETaskResult : uint8
{
	// Task is executed Successly
	ETR_Success,
	// Player Attack Detected While Task
	ETR_AttackDetected,
	// Player is Far Away Enoughly (User Defined)
	ETR_Distanced_Far,
};
