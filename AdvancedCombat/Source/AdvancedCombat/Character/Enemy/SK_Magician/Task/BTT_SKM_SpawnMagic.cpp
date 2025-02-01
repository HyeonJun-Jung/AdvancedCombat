// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SK_Magician/Task/BTT_SKM_SpawnMagic.h"
#include "Character/Enemy/SK_Magician/SK_Magician.h"
#include "Character/CharacterBase_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_SKM_SpawnMagic::UBTT_SKM_SpawnMagic()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("SKM SpawnMagic");
}

EBTNodeResult::Type UBTT_SKM_SpawnMagic::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Magician = Cast<ASK_Magician>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Magician || !Magician->AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	Magician->AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_SKM_SpawnMagic::OnIntrruptedCallback);
	
	// Play Montage
	Magician->MagicAttack(MagicType);

	return EBTNodeResult::InProgress;
}

void UBTT_SKM_SpawnMagic::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsMontagePlaying)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	if (IsMontageIntrrupted)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTT_SKM_SpawnMagic::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_SKM_SpawnMagic::CleanUp()
{
	if (Magician)
	{
		Magician->AnimInst->OnMontageEnded.Clear();
	}
}

void UBTT_SKM_SpawnMagic::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
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

