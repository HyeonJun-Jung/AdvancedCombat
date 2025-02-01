// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Enemy_Base.h"
#include "ACEnums.h"
#include "SK_Magician.generated.h"

UCLASS()
class ADVANCEDCOMBAT_API ASK_Magician : public AEnemy_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* AttackMontage_01;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* AttackMontage_01_Arranged;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* AttackMontage_02;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* BackAttackMontage_01;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* TeleportMontage;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Magic")
	TSubclassOf<AActor> FireBall_C;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Magic")
	TSubclassOf<AActor> FireArrow_C;

	UPROPERTY()
	TObjectPtr<class USKM_AnimInstance> SKM_AnimInst;

	UPROPERTY()
	ESKM_MagicType CurrentMagicType;

protected:
	virtual void BeginPlay();

public:
	void MagicAttack(ESKM_MagicType InMagicType);
	void Teleport();

protected:
	UFUNCTION()
	void SpawnMagic_Hand();

	UFUNCTION()
	void SpawnMagic_Staff();

	void SpawnMagic(FVector& SpawnLocation);

	UFUNCTION()
	void Teleport_Callback();
};
