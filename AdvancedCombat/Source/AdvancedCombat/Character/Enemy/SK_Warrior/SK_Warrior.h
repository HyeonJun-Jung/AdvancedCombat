// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Enemy_Base.h"
#include "SK_Warrior.generated.h"

class UCharacterBase_AnimInstance;

UCLASS()
class ADVANCEDCOMBAT_API ASK_Warrior : public AEnemy_Base
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AM_ComboAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AM_SPAttack;

protected:
	virtual void BeginPlay() override;

public:
	void ComboAttack();
	void SPAttack();

public:
	UCharacterBase_AnimInstance* AnimInst;

};
