// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ACEnums.h"
#include "BTT_Samurai_ComboAttack.generated.h"

UENUM(BlueprintType)
enum class ESamuraiComboType : uint8
{
	Random,
	first,
	second,
	third
};

UCLASS()
class ADVANCEDCOMBAT_API UBTT_Samurai_ComboAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Samurai_ComboAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

public:
	UPROPERTY(EditAnywhere)
	ESamuraiComboType ComboType;

private:
	void CleanUp();

private:
	class UBlackboardComponent* BlackBoard;
	FDelegateHandle LamdaHandle;
	bool IsMontagePlaying = false;
	class ASamurai* Samurai;

public:
	UFUNCTION()
	void OnIntrruptedCallback(class UAnimMontage* Montage, bool Interrupted);
	bool IsMontageIntrrupted = false;

public:
	UFUNCTION()
	void OnAttackParried();

private:
	ETaskResult BBTaskResult = ETaskResult::ETR_Success;
};
