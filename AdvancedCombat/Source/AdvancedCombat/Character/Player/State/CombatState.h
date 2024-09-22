// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatState.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ADVANCEDCOMBAT_API UCombatState : public UObject
{
	GENERATED_BODY()

public:
	UCombatState();
	virtual ~UCombatState();

public:
	virtual void EnterState(class ACombatPlayerCharacter* InCombatCharacter);
	virtual void ExitState() { };
	virtual void SetUpDelegate() { };

// Input List
	
/* Dodge */
public:
	virtual bool IsDodgeable() { return true; };
	virtual void Dodge() { };
	

/* Attack */
public:
	virtual bool IsAttackable() { return true; };
	virtual void Attack() { };
	virtual void DashAttack() { };
	virtual void ResetCombo() { };

	// Related to Notify
	virtual void AttackInputStart() { };
	virtual void CheckShouldAttack() { };


/* Defense */
public:
	bool IsDefenseAble() { return true; };
	void DefenseStart() {};
	void DefenseEnd() {};


/* Damage */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) { return Damage; };

protected:
	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY()
	class ACombatPlayerCharacter* CombatCharacter;

	UPROPERTY()
	class UCombatPlayer_AnimInstance* AnimInst;
};
