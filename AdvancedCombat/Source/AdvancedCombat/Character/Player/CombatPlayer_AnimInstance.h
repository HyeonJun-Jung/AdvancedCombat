// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase_AnimInstance.h"
#include "ACEnums.h"
#include "CombatPlayer_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackInputStart);
DECLARE_MULTICAST_DELEGATE(FCheckDoNextAttack);
DECLARE_MULTICAST_DELEGATE(FDashAttack);
DECLARE_MULTICAST_DELEGATE(FShootMagic_Staff);
DECLARE_MULTICAST_DELEGATE(FShootMagic_Hand);
DECLARE_MULTICAST_DELEGATE(FDashAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackEnd);

class UEquipComponent;

UCLASS()
class ADVANCEDCOMBAT_API UCombatPlayer_AnimInstance : public UCharacterBase_AnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	
public:
	FAttackInputStart Delegate_AttackInputStart;

	UFUNCTION()
	void AnimNotify_AttackInputStart();

	FCheckDoNextAttack Delegate_CheckDoNextAttack;

	UFUNCTION()
	void AnimNotify_CheckDoNextAttack();

	FDashAttack Delegate_DashAttack;

	UFUNCTION()
	void AnimNotify_DashAttack();

	FShootMagic_Staff Delegate_ShootMagic_Staff;

	UFUNCTION()
	void AnimNotify_ShootMagic_Staff();

	FShootMagic_Hand Delegate_ShootMagic_Hand;

	UFUNCTION()
	void AnimNotify_ShootMagic_Hand();

public:
	void SetDefensing(bool value) { bDefensing = value; }
	void SetParried(bool value, EParryType InParryType) 
	{
		Parried = value; NextParryType = InParryType;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDefensing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Parried = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EParryType NextParryType = EParryType::RightToLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UEquipComponent* EquipComponent;

protected:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadOnly)
	FAttackEnd Delegate_AttackEnd;


};
