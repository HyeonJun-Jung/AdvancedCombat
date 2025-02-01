// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase_AnimInstance.h"
#include "SKM_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FSpawnMagic_Staff)
DECLARE_MULTICAST_DELEGATE(FSpawnMagic_Hand)
DECLARE_MULTICAST_DELEGATE(FTeleport)
UCLASS()
class ADVANCEDCOMBAT_API USKM_AnimInstance : public UCharacterBase_AnimInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void AnimNotify_SpawnMagic_Hand();

	UFUNCTION()
	void AnimNotify_SpawnMagic_Staff();

	UFUNCTION()
	void AnimNotify_Teleport();

public:
	FSpawnMagic_Hand Delegate_SpawnMagic_Hand;
	FSpawnMagic_Staff Delegate_SpawnMagic_Staff;
	FTeleport Delegate_Teleport;
};
