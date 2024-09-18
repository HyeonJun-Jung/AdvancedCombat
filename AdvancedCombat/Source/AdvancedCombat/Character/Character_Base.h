// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "ACEnums.h"
#include "Character_Base.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackParried)

UCLASS()
class ADVANCEDCOMBAT_API ACharacter_Base : public ACharacter
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



public:
	// Sets default values for this character's properties
	ACharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Damaged */
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	EACHitReactDirection GetHitReactDirection(const FVector& ImpactPoint, AActor* DamageCauser);
	EACHitReactDirection GetRightOrLeft(const FVector& ImpactPoint, AActor* DamageCauser);

	UFUNCTION()
	virtual void ShowHitReaction(EACHitReactDirection hitDirection);

	UFUNCTION()
	virtual void ShowParriedReaction(EACHitReactDirection hitDirection);

public:
	FAttackParried Delegate_Parried;

public:
	UPROPERTY()
	UAnimInstance* AnimInst;

	UPROPERTY()
	FGameplayTagContainer TagContainer;
};
