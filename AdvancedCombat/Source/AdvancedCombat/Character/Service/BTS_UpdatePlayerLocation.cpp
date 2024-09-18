// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Service/BTS_UpdatePlayerLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UBTS_UpdatePlayerLocation::UBTS_UpdatePlayerLocation()
{
	NodeName = "Update Player Location";
}

void UBTS_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	AActor* owner = OwnerComp.GetOwner();
	if (!blackBoard || !owner) return;

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(blackBoard, 0);
	if (!player) return;

	FVector ownerLocation = owner->GetActorLocation();
	FVector playerLocation = player->GetActorLocation();
	float Dist = FVector::Dist2D(ownerLocation, playerLocation);

	// Update Player Location If In Range.
	if (Dist > MaxDistance)
	{
		blackBoard->SetValueAsObject(FName("Target"), nullptr);
	}
	else
	{
		blackBoard->SetValueAsVector(FName("TargetLocation"), playerLocation);
	}
}
