// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACStructs.h"
#include "ACEnums.h"
#include "AbilitySystemComponent.h"
#include "EquipComponent.generated.h"

class AAbililtyCombatPlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDCOMBAT_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE EWeaponType GetCurrentWeaponType() { return CurrentWeaponType; }

private:
	/*
	*  Abilities behave differently depending on the current weapon state
	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EWeaponType CurrentWeaponType = EWeaponType::EWT_None;

public:	
	// Sets default values for this component's properties
	UEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void EquipItem(const FItemStruct& InItem);

public:
	void ResetCombo();

	UFUNCTION()
	void AttackInputStart();

	UFUNCTION()
	void CheckShouldAttack();

	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	AAbililtyCombatPlayerCharacter* AbililtyCharacter;

public:
	UPROPERTY(BlueprintReadWrite)
	bool bCanAttackInput;
	UPROPERTY(BlueprintReadWrite)
	bool bShouldDoNextAttack;
	UPROPERTY(BlueprintReadWrite)
	int CurrentCombo;
	UPROPERTY(BlueprintReadWrite)
	int MaxCombo;

};
