// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCharacterBase_AnimInstance::UCharacterBase_AnimInstance()
{
}

void UCharacterBase_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (IsValid(Character))
		MovementComp = Character->GetCharacterMovement();
}

void UCharacterBase_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComp && Character)
	{
		GroundSpeed = Character->GetVelocity().Size2D();
		FVector vRotation = UKismetMathLibrary::InverseTransformDirection(Character->GetActorTransform(), Character->GetVelocity());
		FRotator rotation = vRotation.Rotation();
		Angle = rotation.Yaw;
	}

}
