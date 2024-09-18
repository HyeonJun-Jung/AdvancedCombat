// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Samurai_JumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UBTT_Samurai_JumpAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Samurai_JumpAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void CleanUp();

private:
	FDelegateHandle LamdaHandle;
	bool IsMontagePlaying = false;
	class ASamurai* Samurai;

public:
	UFUNCTION()
	void OnIntrruptedCallback(class UAnimMontage* Montage, bool Interrupted);
	bool IsMontageIntrrupted = false;
};
