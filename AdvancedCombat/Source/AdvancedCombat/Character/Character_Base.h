// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "ACEnums.h"
#include "Character_Base.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackParried);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakeDamage);

UCLASS()
class ADVANCEDCOMBAT_API ACharacter_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Fwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Bwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Left;

	// Guard

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Guard", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GuardSuccessMontage_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Guard", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GuardSuccessMontage_Right;

	// Parry

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Parry", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ParryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Parried", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ParriedMontage_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Parried", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ParriedMontage_Right;

	// Hit RootMotion

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Forward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Backward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_GetUp;

	// Hit Inplace

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged|Inplace", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Forward_Inplace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged|Inplace", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Backward_Inplace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged|Inplace", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Left_Inplace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged|Inplace", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_Right_Inplace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage|Damaged|Inplace", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage_HitDown_Inplace;


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
	UFUNCTION(BlueprintCallable)
	void ShowDamage(float DamageAmount);

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UDamageWidgetComponent> DamageNumberClass;*/

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Died();

	EACHitReactDirection GetHitReactDirection(const FVector& ImpactPoint, AActor* DamageCauser);
	EACHitReactDirection GetRightOrLeft(const FVector& ImpactPoint, AActor* DamageCauser);

	UFUNCTION()
	virtual void ShowHitReaction(EACHitReactDirection hitDirection);

	UFUNCTION()
	virtual void ShowHitReaction_Inplace(EACHitReactDirection hitDirection);

	UFUNCTION()
	virtual void ShowParriedReaction(EACHitReactDirection hitDirection);

public:
	FAttackParried Delegate_Parried;

	UPROPERTY(BlueprintAssignable)
	FTakeDamage Delegate_TakeDamage;

public:
	UPROPERTY()
	UAnimInstance* AnimInst;

	UPROPERTY()
	FGameplayTagContainer TagContainer;

public:
	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UACAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UACAttributeSet_Base> AttributeSetBase;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UACGameplayAbility_Base>> DefaultAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

public:
	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);
	virtual void SetStamina(float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina() const;

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMoveSpeedBaseValue() const;

	bool IsAlive() { return bIsAlive; }

protected:
	bool bIsAlive = true;
};
