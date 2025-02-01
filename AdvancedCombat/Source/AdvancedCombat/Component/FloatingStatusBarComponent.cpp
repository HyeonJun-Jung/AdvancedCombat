// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FloatingStatusBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UFloatingStatusBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if(PCM)
		SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), PCM->GetCameraLocation()));
}
