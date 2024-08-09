// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ACBase.generated.h"


UCLASS()
class ADVANCEDCOMBAT_API UBTTask_ACBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	const FName BBKey_Target = FName("Target");
	const FName BBKey_TargetLocation = FName("TargetLocation");
	const FName BBKey_TargetDistance = FName("TargetDistance");
};
