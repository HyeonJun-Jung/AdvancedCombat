// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedCombatCharacter.h"
#include "CombatPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API ACombatPlayerCharacter : public AAdvancedCombatCharacter
{
	GENERATED_BODY()

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DefenseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;

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
	ACombatPlayerCharacter();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

private:
	class APlayerController* PlayerController;
	class UCombatPlayer_AnimInstance* CombatAnimInst;

	/* Movement Input */
private:
	void DisableMovementInput(float Duration);

private:
	FTimerHandle TimerHandle_DisableMovement;


	/* Montage Ended */
public:
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);


	/* Jump */
public:
	bool IsJumpable();
	virtual void Jump() override;
	virtual void StopJumping() override;
	
	UFUNCTION()
	void Landed(const FHitResult& hit);


	/* Lock On */
public:
	void LockOn();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULockOnComponent* LockOnComponent;


	/* Dodge */
public:
	bool IsDodgeable();
	void Dodge();


	/* Attack */
public:
	bool IsAttackable();

	void Attack();
	void DashAttack();

	UFUNCTION()
	void AttackInputStart();

	UFUNCTION()
	void CheckShouldAttack();

	void ResetCombo();

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


	/* Damaged */
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DealWithSamurai(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, class ASamurai* Samurai);
	void EnableInvincible(float Duration);

	bool bInvincible = false;
	FTimerHandle TimerHandle_Invincible;
};
