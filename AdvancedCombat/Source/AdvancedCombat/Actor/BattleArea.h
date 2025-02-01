// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleArea.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOutOfArea, AActor*);

class UBoxComponent;

UCLASS()
class ADVANCEDCOMBAT_API ABattleArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FOutOfArea Delegate_OutOfArea;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* AreaBox;
};
