// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SK_Warrior/Task/BTT_SKW_SPAttack.h"
#include "Character/Enemy/SK_Warrior/SK_Warrior.h"
#include "Character/CharacterBase_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_SKW_SPAttack::UBTT_SKW_SPAttack()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("SKW SPAttack");
}

EBTNodeResult::Type UBTT_SKW_SPAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Warrior = Cast<ASK_Warrior>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Warrior || !Warrior->AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	Warrior->AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_SKW_SPAttack::OnIntrruptedCallback);

	// Play Montage
	Warrior->SPAttack();

	return EBTNodeResult::InProgress;
}

void UBTT_SKW_SPAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsMontagePlaying)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_SKW_SPAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_SKW_SPAttack::CleanUp()
{
	if (Warrior)
	{
		Warrior->AnimInst->OnMontageEnded.Clear();
	}
}

void UBTT_SKW_SPAttack::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
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
