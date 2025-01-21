// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageTypes.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API UNormalDamage : public UDamageType
{
	GENERATED_BODY()

};

UCLASS()
class ADVANCEDCOMBAT_API UGuardableDamage : public UDamageType
{
	GENERATED_BODY()
	
};

UCLASS()
class ADVANCEDCOMBAT_API UParryableDamage : public UDamageType
{
	GENERATED_BODY()

};

UCLASS()
class ADVANCEDCOMBAT_API UUnbreakableDamage : public UDamageType
{
	GENERATED_BODY()

};

UCLASS()
class ADVANCEDCOMBAT_API UStingDamage : public UDamageType
{
	GENERATED_BODY()

};

UCLASS()
class ADVANCEDCOMBAT_API UAirborneDamage : public UDamageType
{
	GENERATED_BODY()

};

UCLASS()
class ADVANCEDCOMBAT_API UInAirDamage : public UDamageType
{
	GENERATED_BODY()

};

UCLASS()
class ADVANCEDCOMBAT_API UHitDownDamage : public UDamageType
{
	GENERATED_BODY()

};