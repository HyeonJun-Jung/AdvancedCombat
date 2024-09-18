// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase_AnimInstance.h"
#include "Samurai_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FSpawnSlash)
DECLARE_MULTICAST_DELEGATE(FSpawnIdleSlash)

UCLASS()
class ADVANCEDCOMBAT_API USamurai_AnimInstance : public UCharacterBase_AnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AnimNotify_SpawnSlash();

	UFUNCTION()
	void AnimNotify_SpawnIdleSlash();

	FSpawnSlash Delegate_SpawnSlash;
	FSpawnIdleSlash Delegate_SpawnIdleSlash;
};
