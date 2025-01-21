// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolRoute.generated.h"

UCLASS()
class ADVANCEDCOMBAT_API APatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	float GetPatrolLength();

public:
	FVector GetPatrolLocationAtDistance(float length);
	FVector GetNextPatrolLocation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USplineComponent* spline;

	UPROPERTY(BlueprintReadOnly)
	float Duration;
};
