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

class ULockOnComponent;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UInputMappingContext;
class UInputAction;
class UEquipComponent;
struct FInputActionValue;

UCLASS()
class ADVANCEDCOMBAT_API AAbililtyCombatPlayerCharacter : public ACharacter_Base
{
	GENERATED_BODY()

public:
	/** LockOn Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	ULockOnComponent* LockOnComponent;

	/** Component For Equipment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UEquipComponent* EquipComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftWeapon_Static;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LeftWeapon_Skeletal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightWeapon_Static;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RightWeapon_Skeletal;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Defense Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DefenseAction;

	/** Weapon Ability Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WeaponAbilityAction;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

public:
	AAbililtyCombatPlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

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
	/** Returns Combat AnimInstanc **/
	FORCEINLINE class UCombatPlayer_AnimInstance* GetCombatAnimInst() const { return CombatAnimInst; }

public:
	void BindDelegateFunctions();

protected:
	UPROPERTY()
	class UCombatPlayer_AnimInstance* CombatAnimInst;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	void SetupGASInputComponent();

	// GAS Input Function
	void GASInputPressed(EACAbilityInputID InputId);
	void GASInputReleased(EACAbilityInputID InputId);

	// Set Default Input Abilities 
	void SetDefaultInputAbilities();

	// Set Default Abilities
	void SetDefaultAbilities();

	// Bind TargetData Confirm & Cancel Input
	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

public:
	// HP, MP, Stamina Updated
	void BindStatusWidgetFunction(class UWidget_Status* InWidget);

	// Magic
protected:
	UFUNCTION()
	void MagicStaff_Callback();

	UFUNCTION()
	void MagicHand_Callback();

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
