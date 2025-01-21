// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PatrolRoute.h"
#include "Components/SplineComponent.h"

// Sets default values
APatrolRoute::APatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	spline = CreateDefaultSubobject<USplineComponent>(FName("SplineComponent"));
	spline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
}

float APatrolRoute::GetPatrolLength()
{
	if(!spline)
		return 0.0f;
	return spline->GetSplineLength();
}

FVector APatrolRoute::GetPatrolLocationAtDistance(float length)
{
	if (!spline) return FVector();
	
	return spline->GetLocationAtDistanceAlongSpline(length, ESplineCoordinateSpace::World);
}