// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Task/BTTask_ACBase.h"
#include "BTT_SKW_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UBTT_SKW_Attack : public UBTTask_ACBase
{
	GENERATED_BODY()

public:
	UBTT_SKW_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void CleanUp();

private:
	FDelegateHandle LamdaHandle;
	bool IsMontagePlaying = false;
	class ASK_Warrior* Warrior;

public:
	UFUNCTION()
	void OnIntrruptedCallback(class UAnimMontage* Montage, bool Interrupted);
	bool IsMontageIntrrupted = false;
};
