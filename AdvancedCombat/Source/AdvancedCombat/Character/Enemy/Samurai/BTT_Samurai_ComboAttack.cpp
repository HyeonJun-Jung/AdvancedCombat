// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Samurai/BTT_Samurai_ComboAttack.h"
#include "Character/Enemy/Samurai/Samurai.h"
#include "Character/Enemy/Samurai/Samurai_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_Samurai_ComboAttack::UBTT_Samurai_ComboAttack()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("Samurai ComboAttack");
}

EBTNodeResult::Type UBTT_Samurai_ComboAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BlackBoard = OwnerComp.GetBlackboardComponent();

	Samurai = Cast<ASamurai>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Samurai || !Samurai->AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	BBTaskResult = ETaskResult::ETR_Success;
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Play Montage
	Samurai->ComboAttack(static_cast<int>(ComboType));

	// Binding Function
	Samurai->AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_Samurai_ComboAttack::OnIntrruptedCallback);
	Samurai->Delegate_Parried.AddDynamic(this, &UBTT_Samurai_ComboAttack::OnAttackParried);

	return EBTNodeResult::InProgress;
}

void UBTT_Samurai_ComboAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsMontagePlaying)
	{
		if (BlackBoard)
		{
			BlackBoard->SetValueAsEnum(FName("TaskResult"), static_cast<int>(BBTaskResult));
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_Samurai_ComboAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_Samurai_ComboAttack::CleanUp()
{
	Samurai->AnimInst->OnMontageEnded.Clear();
	Samurai->Delegate_Parried.Clear();
}

void UBTT_Samurai_ComboAttack::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("%s : Montage Ended or Interrupted."), *GetName());
	if (Interrupted)
	{
		IsMontageIntrrupted = true;
	}
	else
	{
		IsMontagePlaying = false;
	}
}

void UBTT_Samurai_ComboAttack::OnAttackParried(EACHitReactDirection hitDirection)
{
	UE_LOG(LogTemp, Warning, TEXT("%s : Attack Parried."), *GetName());
	IsMontagePlaying = false;
	BBTaskResult = ETaskResult::ETR_Parried;
	Samurai->ShowParriedReaction(hitDirection);
}

