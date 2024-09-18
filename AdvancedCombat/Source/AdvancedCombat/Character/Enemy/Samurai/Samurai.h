// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Enemy_Base.h"
#include "Samurai.generated.h"

UCLASS()
class ADVANCEDCOMBAT_API ASamurai : public AEnemy_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Attack")
	UAnimMontage* Montage_NormalAttack_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Attack")
	UAnimMontage* Montage_NormalAttack_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Attack")
	UAnimMontage* Montage_AttackCombo01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Attack")
	UAnimMontage* Montage_AttackCombo02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Attack")
	UAnimMontage* Montage_AttackCombo03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	UAnimMontage* Montage_SPAttack01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	UAnimMontage* Montage_SPAttack02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	UAnimMontage* Montage_SPAttack03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	UAnimMontage* Montage_SPAttack04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	UAnimMontage* Montage_SPAttack05;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	TSubclassOf<class AACProjectile> SlashActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	TSubclassOf<class AActor> IdleSlashActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Special Attack")
	float Slash_AdditionalRoll = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage|Jump Attack")
	UAnimMontage* Montage_JumpAttack;

	
protected:
	virtual void BeginPlay() override;

public:
	class USamurai_AnimInstance* SamuraiAnimInst;


	/* Attack */
public:
	void NormalAttack_Left();
	void NormalAttack_Right();

	// Attack
	void RandomComboAttack();
	void ComboAttack(int ComboIdx);

	// Special Attack
	void SpecialAttack(int AttackIdx);
	void JumpAttack();
	void StingFailed();

	UFUNCTION()
	void SpawnSlash();

	UFUNCTION()
	void SpawnIdleSlash();

	// Dodge
	void Dodge_Bwd();


	/* Damaged */
public:
	bool IsParryable();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DealWithCombatCharacter(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, class ACombatPlayerCharacter* CombatCharacter);
};
