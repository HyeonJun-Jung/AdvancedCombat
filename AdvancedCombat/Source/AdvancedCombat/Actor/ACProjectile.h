// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACProjectile.generated.h"

UCLASS()
class ADVANCEDCOMBAT_API AACProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void AddForceToProjectile(FVector Force);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	float LifeTime = 5.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ForceIntensity = 1000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

};
