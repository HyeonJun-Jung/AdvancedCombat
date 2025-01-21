// Fill out your copyright notice in the Description page of Project Settings.


#include "ACAssetManager.h"
#include "ACGameplayTags.h"

UACAssetManager& UACAssetManager::Get()
{
	check(GEngine);
	UACAssetManager* assetManager = Cast<UACAssetManager>(GEngine->AssetManager);
	return *assetManager;
}

void UACAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FACGameplayTags::InitializeNativeGameplayTags();
}

