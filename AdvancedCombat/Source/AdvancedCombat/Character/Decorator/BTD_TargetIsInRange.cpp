// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Decorator/BTD_TargetIsInRange.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "AIController.h"

UBTD_TargetIsInRange::UBTD_TargetIsInRange()
{
	NodeName = TEXT("Target Is In Range");
}

bool UBTD_TargetIsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto owningPawn =  OwnerComp.GetAIOwner()->GetPawn();
	APawn* target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!owningPawn || !target) return false;

	float dist = FVector::Dist(owningPawn->GetActorLocation(), target->GetActorLocation());
	return (dist < Range);
}
