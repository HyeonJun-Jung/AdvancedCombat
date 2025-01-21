// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ACAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UACAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UACAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
