// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SK_Warrior/Task/BTT_SKW_Attack.h"
#include "Character/Enemy/SK_Warrior/SK_Warrior.h"
#include "Character/CharacterBase_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_SKW_Attack::UBTT_SKW_Attack()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("SKW ComboAttack");
}

EBTNodeResult::Type UBTT_SKW_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Warrior = Cast<ASK_Warrior>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Warrior || !Warrior->AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	Warrior->AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_SKW_Attack::OnIntrruptedCallback);

	// Play Montage
	Warrior->ComboAttack();

	return EBTNodeResult::InProgress;
}

void UBTT_SKW_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsMontagePlaying)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_SKW_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_SKW_Attack::CleanUp()
{
	if (Warrior)
	{
		Warrior->AnimInst->OnMontageEnded.Clear();
	}
}

void UBTT_SKW_Attack::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
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
