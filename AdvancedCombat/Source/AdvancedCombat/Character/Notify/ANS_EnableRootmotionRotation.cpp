// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_EnableRootmotionRotation.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_EnableRootmotionRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!IsValid(character) || !character->GetMovementComponent()) return;
	character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

void UANS_EnableRootmotionRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (!IsValid(character) || !IsValid(character->GetMovementComponent())) return;
	character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
}
