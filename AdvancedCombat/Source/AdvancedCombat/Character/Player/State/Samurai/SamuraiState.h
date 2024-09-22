// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Player/State/CombatState.h"
#include "SamuraiState.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ADVANCEDCOMBAT_API USamuraiState : public UCombatState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Fwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Bwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Guard", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GuardSuccessMontage_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Guard", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GuardSuccessMontage_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Parry", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ParryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Parried", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ParriedMontage_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Parried", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ParriedMontage_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Right;

	/** Combat Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Attack", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage_01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Attack", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage_02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Attack", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage_03;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Parry", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* StingParryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Dodge Attack", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_DodgeAttack;
	
public:
	USamuraiState();
	virtual ~USamuraiState();

public:
	virtual void EnterState(class ACombatPlayerCharacter* InCombatCharacter);
	virtual void ExitState();
	virtual void SetUpDelegate();

	// Input List

	/* Dodge */
public:
	virtual bool IsDodgeable();
	virtual void Dodge();


	/* Attack */
public:
	virtual bool IsAttackable();
	virtual void Attack();
	virtual void DashAttack();
	virtual void ResetCombo();

	// Related to Notify
	virtual void AttackInputStart();
	virtual void CheckShouldAttack();

private:
	int MaxCombo = 4;
	int CurrentCombo = 0;
	bool bCanAttackInput = false;
	bool bShouldDoNextAttack = false;
	bool bNextAttackChecking = false;
	bool bShouldDoDashAttack = false;


	/* Defense */
public:
	bool IsDefenseAble();
	void DefenseStart();
	void DefenseEnd();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ParryTime = 0.2f;

	FTimerHandle TimerHandle_Parryable;


	/* Damage */
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	void DealWithSamurai(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, class ASamurai* Samurai);

};
