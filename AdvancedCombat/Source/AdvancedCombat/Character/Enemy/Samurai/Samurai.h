// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Enemy_Base.h"
#include "Samurai.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDCOMBAT_API ASamurai : public AEnemy_Base
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	class USamurai_AnimInstance* AnimInst;

public:
	void NormalAttack_Left();
	void NormalAttack_Right();

	void RandomComboAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* Montage_NormalAttack_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* Montage_NormalAttack_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* Montage_AttackCombo01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* Montage_AttackCombo02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* Montage_AttackCombo03;
};
