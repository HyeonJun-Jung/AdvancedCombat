// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase_AnimInstance.h"
#include "Samurai_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FSpawnSlash)

UCLASS()
class ADVANCEDCOMBAT_API USamurai_AnimInstance : public UCharacterBase_AnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AnimNotify_SpawnSlash();

	FSpawnSlash Delegate_SpawnSlash;
};
