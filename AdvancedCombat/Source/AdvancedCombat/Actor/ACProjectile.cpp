// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ACProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AACProjectile::AACProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AACProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACProjectile::AddForceToProjectile(FVector Force)
{
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->AddForce(Force * ForceIntensity);
}

