// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BattleArea.h"
#include "Components/BoxComponent.h"

// Sets default values
ABattleArea::ABattleArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AreaBox = CreateDefaultSubobject<UBoxComponent>(FName("Area"));
	AreaBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABattleArea::BeginPlay()
{
	Super::BeginPlay();
	
	AreaBox->OnComponentEndOverlap.AddDynamic(this, &ABattleArea::OnOverlapEnded);
}

// Called every frame
void ABattleArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleArea::OnOverlapEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Delegate_OutOfArea.Broadcast(OtherActor);
}

