// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Task/BTTask_ACBase.h"
#include "BTT_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UBTT_Patrol : public UBTTask_ACBase
{
	GENERATED_BODY()

public:
	UBTT_Patrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void PatrolEnded();

private:
	class UBlackboardComponent* BlackBoard;
	bool bPatrolEnded = false;
};
