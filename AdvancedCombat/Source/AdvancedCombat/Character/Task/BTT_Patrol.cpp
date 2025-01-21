// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Task/BTT_Patrol.h"
#include "Character/Enemy/EnemyBase_AIController.h"
#include "Actor/PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Component/PatrolComponent.h"

UBTT_Patrol::UBTT_Patrol()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("Patrol");
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	bPatrolEnded = false;
	BlackBoard = OwnerComp.GetBlackboardComponent();

	AEnemyBase_AIController* AC = Cast<AEnemyBase_AIController>(OwnerComp.GetAIOwner());
	if (!AC) return EBTNodeResult::Failed;

	UPatrolComponent* patrolComp = AC->GetComponentByClass<UPatrolComponent>();
	if(!patrolComp) return EBTNodeResult::Failed;

	patrolComp->Delegate_PatrolEnded.Unbind();
	patrolComp->Delegate_PatrolEnded.BindUObject(this, &UBTT_Patrol::PatrolEnded);

	bool success = AC->StartPatrol();
	if (!success)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bPatrolEnded)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_Patrol::PatrolEnded()
{
	bPatrolEnded = true;
}
