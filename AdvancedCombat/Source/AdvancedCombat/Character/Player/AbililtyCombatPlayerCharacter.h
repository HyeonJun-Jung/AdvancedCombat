// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedCombatCharacter.h"
#include "GameFramework/Character.h"
#include "Character/Character_Base.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "ACEnums.h"
#include "GameplayEffectTypes.h"
#include "Delegates/Delegate.h"
#include "AbililtyCombatPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ADVANCEDCOMBAT_API AAbililtyCombatPlayerCharacter : public ACharacter_Base
{
	GENERATED_BODY()

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComp;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Weapon Ability Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WeaponAbilityAction;

public:
	AAbililtyCombatPlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Interact input */
	void Interact();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UPROPERTY()
	class UCombatPlayer_AnimInstance* CombatAnimInst;


public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	void SetupGASInputComponent();

	//// Apply GASInputDataAsset 
	//void GiveGASAbilities(UGASInputDataAsset* InAbilityAsset);
	//void RemoveGASAbilities(UGASInputDataAsset* InAbilityAsset);

	// GAS Input Function
	void GASInputPressed(EACAbilityInputID InputId);
	void GASInputReleased(EACAbilityInputID InputId);

	// Set Default Input Abilities 
	void SetDefaultInputAbilities();

	// Bind TargetData Confirm & Cancel Input
	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

protected:
	void LevelUp_Callback(const FOnAttributeChangeData& AttributeChangeData);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> LevelUpEffect;

protected:
	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = GAS)
	TMap<EACAbilityInputID, TSubclassOf<class UACGameplayAbility_Base>> DefaultInputAbilities;

	bool ASCInputBound = false;
};
