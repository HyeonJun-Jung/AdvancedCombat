// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#define TAG_CHARACTER_STATE_JUMPING FGameplayTag::RequestGameplayTag(FName("Character.State.Jumping"))
#define TAG_CHARACTER_STATE_ATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.Attacking"))
#define TAG_CHARACTER_STATE_DEFENSING FGameplayTag::RequestGameplayTag(FName("Character.State.Defensing"))
#define TAG_CHARACTER_STATE_DODGING FGameplayTag::RequestGameplayTag(FName("Character.State.Dodging"))
#define TAG_CHARACTER_STATE_DAMAGED FGameplayTag::RequestGameplayTag(FName("Character.State.Damaged"))
#define TAG_CHARACTER_STATE_PARRYABLE FGameplayTag::RequestGameplayTag(FName("Character.State.Parryable"))