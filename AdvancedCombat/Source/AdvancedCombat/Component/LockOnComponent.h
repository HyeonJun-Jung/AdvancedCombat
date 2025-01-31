// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDCOMBAT_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void LockOn();

private:
	void SetRoationSetting(bool bLockOn);

public:
	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, Category = Debug)
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere, Category = BindAxis)
	bool pitch = false;

	UPROPERTY(EditAnywhere, Category = BindAxis)
	bool yaw = false;

	UPROPERTY(EditAnywhere, Category = BindAxis)
	bool roll = false;
	
private:
	class ACharacter* player;
	class APlayerController* playerController;
};
