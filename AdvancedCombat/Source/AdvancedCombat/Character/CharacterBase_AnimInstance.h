// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterBase_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UCharacterBase_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCharacterBase_AnimInstance();

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float GroundSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float Angle = 0.f;

	UPROPERTY(BlueprintReadOnly, category = "Movement")
	class UCharacterMovementComponent* MovementComp;

protected:
	class ACharacter* Character;
};
