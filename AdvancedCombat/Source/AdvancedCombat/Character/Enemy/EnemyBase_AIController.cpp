// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyBase_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AEnemyBase_AIController::AEnemyBase_AIController(FObjectInitializer const& object_initializer)
{
	PrimaryActorTick.bCanEverTick = true;

	BTComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	SetAIPerception();
}

void AEnemyBase_AIController::BeginPlay()
{
	Super::BeginPlay();

	// SetAIPerception();

	if (IsValid(BehaviorTree) && IsValid(BTComponent) && IsValid(Blackboard))
	{
		RunBehaviorTree(BehaviorTree);
		BTComponent->StartTree(*BehaviorTree);
		Blackboard->SetValueAsFloat(BBKey_TargetDistance, 10000);
	}
}

void AEnemyBase_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast<UBehaviorTreeComponent>(BrainComponent)
	if (IsValid(Blackboard) && IsValid(BehaviorTree))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("AEnemyBase_AIController : Blackboard is not valid."));

}
void AEnemyBase_AIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyBase_AIController : Blackboard is not valid."));
		return;
	}

	if (Stimulus.Type == SightConfig->GetSenseID())
	{
		if (actor == UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s Sight : Player Detected."), *GetName());
				Blackboard->SetValueAsObject(BBKey_Target, actor);
				Blackboard->SetValueAsVector(BBKey_TargetLocation, actor->GetActorLocation());
			}
		}
	}
	else if (Stimulus.Type == HearingConfig->GetSenseID())
	{
		if (actor == UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s Hearing : Player Detected."), *GetName());
				Blackboard->SetValueAsObject(BBKey_Target, actor);
				Blackboard->SetValueAsVector(BBKey_TargetLocation, actor->GetActorLocation());
			}
		}
	}

}

void AEnemyBase_AIController::SetAIPerception()
{
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	HearingConfig->HearingRange = 3000.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->SetMaxAge(35.0f);

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyBase_AIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}

