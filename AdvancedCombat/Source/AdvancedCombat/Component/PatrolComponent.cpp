// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PatrolComponent.h"
#include "Actor/PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/Enemy_Base.h"

// Sets default values for this component's properties
UPatrolComponent::UPatrolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	patrolTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Timeline"));

	InterpPatrolFloat.BindUFunction(this, FName("PatrolTimelineUpdated"));
	InterpPatrolFinished.BindUFunction(this, FName("PatrolTimelineFinished"));
}


// Called when the game starts
void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!patrolfCurve)
		return;

	patrolTimeline->AddInterpFloat(patrolfCurve, InterpPatrolFloat, TEXT("Alpha"));
	patrolTimeline->SetTimelineFinishedFunc(InterpPatrolFinished);
	patrolTimeline->SetLooping(false);
	patrolTimeline->SetIgnoreTimeDilation(true);
}


// Called every frame
void UPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPatrolComponent::StartPatrol(float MaxWalkSpeed, UBlackboardComponent* InBlackBoard)
{
	AController* AC = Cast<AController>(GetOwner());
	if (!AC) return false;
	AEnemy_Base* enemy = Cast<AEnemy_Base>(AC->GetPawn());
	if (!enemy) return false;

	patrolRoute = enemy->GetPatrolRoute();
	if (!patrolRoute || !InBlackBoard) return false;

	BlackBoard = InBlackBoard;
	duration = patrolRoute->GetPatrolLength() / MaxWalkSpeed;
	patrolTimeline->SetPlayRate(1.f / duration);

	if (CurrentState == EPatrolState::EINCLINE)
		patrolTimeline->PlayFromStart();
	else
		patrolTimeline->ReverseFromEnd();

	return true;
}

void UPatrolComponent::StopPatrol()
{
	patrolTimeline->Stop();
}

void UPatrolComponent::PatrolTimelineUpdated(float value)
{
	if (!BlackBoard || !patrolRoute) return;
	
	float dist = FMath::Lerp(0.f, patrolRoute->GetPatrolLength(), value);

	BlackBoard->SetValueAsVector(FName("PatrolPoint"), patrolRoute->GetPatrolLocationAtDistance(dist));
}

void UPatrolComponent::PatrolTimelineFinished()
{
	CurrentState = CurrentState == EPatrolState::EINCLINE ? EPatrolState::EDECLINE : EPatrolState::EINCLINE;
	if (Delegate_PatrolEnded.IsBound())
		Delegate_PatrolEnded.Execute();
}

