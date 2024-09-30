// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interact_Interface.h"
#include "Item_Base.generated.h"

UCLASS()
class ADVANCEDCOMBAT_API AItem_Base : public AActor, public IInteract_Interface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemDataRow;
	
public:	
	// Sets default values for this actor's properties
	AItem_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_With_Implementation(class UInventoryComponent* InventoryComponent) override;

};
