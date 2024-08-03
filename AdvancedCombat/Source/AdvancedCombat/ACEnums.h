// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETaskResult : uint8
{
	ETR_Success,
	ETR_Attacked,
	ETR_Parried,
};
