// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/Notify_CameraShake.h"
#include "Kismet/GameplayStatics.h"

void UNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(MeshComp, 0);
	if (!controller) return;

	controller->ClientStartCameraShake(CameraShakeClass, ShakingScale);
}
