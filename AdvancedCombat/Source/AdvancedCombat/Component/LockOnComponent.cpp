// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/LockOnComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<ACharacter>(GetOwner());
	if (player)
	{
		playerController = Cast<APlayerController>(player->GetController());
	}
}


// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!player || !playerController)
		return;

	if (TargetActor)
	{
		FVector TowardVector = TargetActor->GetActorLocation() - player->GetActorLocation();
		FRotator TowardRotation = TowardVector.ToOrientationRotator();
		FRotator playerRotation = playerController->GetControlRotation();

		if (pitch) playerRotation.Pitch = TowardRotation.Pitch;
		if (yaw) playerRotation.Yaw = TowardRotation.Yaw;
		if (roll) playerRotation.Roll = TowardRotation.Roll;

		playerController->SetControlRotation(playerRotation);
	}
}

void ULockOnComponent::LockOn()
{
	if (TargetActor)
	{
		TargetActor = nullptr;
		return;
	}

	AActor* owner = GetOwner();
	if (!owner) return;
	FVector ownerLocation = owner->GetActorLocation();

	TArray<FHitResult> hitResults;

	// Trace Types
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(pawn);

	// Actors To Ignore
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(owner);

	// Sphere Trace
	UKismetSystemLibrary::SphereTraceMultiForObjects(owner,
		ownerLocation, ownerLocation, 1000,
		ObjectTypes, false, ActorsToIgnore, DebugType.GetValue(), hitResults, true);

	// Get nearest Actor
	AActor* nearestActor = nullptr;
	float minDistance = 10000.f;
	for (auto& hitResult : hitResults)
	{
		AActor* hitActor = hitResult.GetActor();
		if (!hitActor)
			continue;
		float distance = FVector::Dist2D(owner->GetActorLocation(), hitActor->GetActorLocation());
		if (distance < minDistance)
		{
			nearestActor = hitActor;
			minDistance = distance;
		}
	}

	// Set TargetActor
	TargetActor = nearestActor;
}

