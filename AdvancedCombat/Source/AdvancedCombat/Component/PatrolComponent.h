// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "PatrolComponent.generated.h"

enum EPatrolState
{
	EINCLINE,
	EDECLINE,
};

DECLARE_DELEGATE(FPatrolEnded)

class APatrolRoute;
class UTimelineComponent;
class UCurveFloat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDCOMBAT_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	bool StartPatrol(float MaxWalkSpeed, class UBlackboardComponent* InBlackBoard);

	UFUNCTION(BlueprintCallable)
	void StopPatrol();

protected:
	UFUNCTION()
	void PatrolTimelineUpdated(float value);

	UFUNCTION()
	void PatrolTimelineFinished();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<APatrolRoute> patrolRoute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTimelineComponent* patrolTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* patrolfCurve;
	
private:
	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> BlackBoard;

	FOnTimelineFloat InterpPatrolFloat;
	FOnTimelineEvent InterpPatrolFinished;

	EPatrolState CurrentState = EPatrolState::EINCLINE;
	float duration;

public:
	FPatrolEnded Delegate_PatrolEnded;
};
