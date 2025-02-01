// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_TargetIsInRange.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UBTD_TargetIsInRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTD_TargetIsInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere)
	float Range = 150.f;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetKey;
};
