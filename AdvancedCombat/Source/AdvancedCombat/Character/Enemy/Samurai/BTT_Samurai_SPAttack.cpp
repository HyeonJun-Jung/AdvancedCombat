// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Samurai/BTT_Samurai_SPAttack.h"
#include "Character/Enemy/Samurai/Samurai.h"
#include "Character/Enemy/Samurai/Samurai_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_Samurai_SPAttack::UBTT_Samurai_SPAttack()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("Samurai Special Attack");
}

EBTNodeResult::Type UBTT_Samurai_SPAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Samurai = Cast<ASamurai>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Samurai || !Samurai->AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Play Montage
	Samurai->SpecialAttack(static_cast<int>(SpecialAttackType) + 1);

	// Binding Function
	Samurai->AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_Samurai_SPAttack::OnIntrruptedCallback);
	Samurai->Delegate_Parried.AddDynamic(this, &UBTT_Samurai_SPAttack::OnAttackParried);

	return EBTNodeResult::InProgress;
}

void UBTT_Samurai_SPAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsMontagePlaying)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_Samurai_SPAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_Samurai_SPAttack::CleanUp()
{
	Samurai->AnimInst->OnMontageEnded.Clear();
	Samurai->Delegate_Parried.Clear();
}

void UBTT_Samurai_SPAttack::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
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

void UBTT_Samurai_SPAttack::OnAttackParried(EACHitReactDirection hitDirecton)
{
	UE_LOG(LogTemp, Warning, TEXT("%s : Attack Parried."), *GetName());
	IsMontagePlaying = false;
	Samurai->ShowParriedReaction(hitDirecton);
}


