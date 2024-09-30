// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/AbililtyCombatPlayerCharacter.h"
#include "Character/Player/CombatPlayer_AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/Player/ACPlayerState.h"
#include "Character/Ability/ACAbilitySystemComponent.h"
#include "Character/Ability/ACGameplayAbility_Base.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Ability/AttributeSets/ACAttributeSet_Base.h"
#include "Blueprint/WidgetTree.h"
#include "GameplayEffectTypes.h"
#include "Character/Ability/ACGameplayEffect_Base.h"
#include "Character/Ability/ACGameplayAbility_Base.h"
#include "Component/InventoryComponent.h"

//////////////////////////////////////////////////////////////////////////
// AAbililtyCombatPlayerCharacter

AAbililtyCombatPlayerCharacter::AAbililtyCombatPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a Inventory
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


}

void AAbililtyCombatPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AACPlayerState* playerState = NewController->GetPlayerState<AACPlayerState>();
	if (playerState)
	{
		ASC = Cast<UACAbilitySystemComponent>(playerState->GetAbilitySystemComponent());
		ASC->InitAbilityActorInfo(playerState, this);

		AttributeSetBase = playerState->GetAttributeSet();
		
		InitializeAttributes();

		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());

		AddStartupEffects();

		AddCharacterAbilities();

		SetDefaultInputAbilities();

		APlayerController* playerController = CastChecked<APlayerController>(NewController);
		playerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AAbililtyCombatPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Get AnimInstance
	CombatAnimInst = Cast<UCombatPlayer_AnimInstance>(GetMesh()->GetAnimInstance());

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Bind Attribute Delegate
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCharacterLevelAttribute()).AddUObject(this, &AAbililtyCombatPlayerCharacter::LevelUp_Callback);
}

UAbilitySystemComponent* AAbililtyCombatPlayerCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAbililtyCombatPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAbililtyCombatPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAbililtyCombatPlayerCharacter::Look);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAbililtyCombatPlayerCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	SetupGASInputComponent();
	BindASCInput();
}

void AAbililtyCombatPlayerCharacter::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAbililtyCombatPlayerCharacter::GASInputPressed, EACAbilityInputID::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AAbililtyCombatPlayerCharacter::GASInputReleased, EACAbilityInputID::Jump);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAbililtyCombatPlayerCharacter::GASInputPressed, EACAbilityInputID::Attack);
		EnhancedInputComponent->BindAction(WeaponAbilityAction, ETriggerEvent::Triggered, this, &AAbililtyCombatPlayerCharacter::GASInputPressed, EACAbilityInputID::Ability01);
	}
}

void AAbililtyCombatPlayerCharacter::SetDefaultInputAbilities()
{
	for (const auto& StartInputAbility : DefaultInputAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		StartSpec.InputID = static_cast<int32>(StartInputAbility.Key);
		ASC->GiveAbility(StartSpec);
	}
}
//
//void AAbililtyCombatPlayerCharacter::GiveGASAbilities(UGASInputDataAsset* InAbilityAsset)
//{
//	if (IsValid(ASC) && IsValid(InputComponent))
//	{
//		APlayerController* PlayerController = Cast<APlayerController>(Controller);
//		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
//		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
//
//		if (InAbilityAsset && InputSubsystem)
//		{
//			// if Asset has MappingContext, Add Mapping Context
//			if (InAbilityAsset->GetMappingContext())
//			{
//				InputSubsystem->ClearAllMappings();
//				InputSubsystem->AddMappingContext(InAbilityAsset->GetMappingContext(), 0);
//			}
//
//			// 1. Give Input Abilities
//			const TSet<FGameplayInputAbilityInfo>& AbilitySet = InAbilityAsset->GetInputAbilities();
//			for (const auto& Ability : AbilitySet)
//			{
//				// Give Ability to ASC
//				FGameplayAbilitySpec StartSpec(Ability.GameplayAbilityClass);
//				StartSpec.InputID = static_cast<int32>(Ability.InputID);
//				ASC->GiveAbility(StartSpec);
//
//				// Add Enhanced Input Binding
//				if (Ability.InputAction)
//				{
//					EnhancedInputComponent->BindAction(Ability.InputAction, ETriggerEvent::Started, this, &AAbililtyCombatPlayerCharacter::GASInputPressed, Ability.InputID);
//					EnhancedInputComponent->BindAction(Ability.InputAction, ETriggerEvent::Completed, this, &AAbililtyCombatPlayerCharacter::GASInputReleased, Ability.InputID);
//				}
//			}
//
//			// 2. Give Defualt Abilities
//			const TArray<TSubclassOf<class UGPGameplayAbility_Base>> defaultAbilities = InAbilityAsset->GetDefaultAbilities();
//			for (const auto& Ability : DefaultAbilities)
//			{
//				// Give Ability to ASC
//				FGameplayAbilitySpec StartSpec(Ability);
//				ASC->GiveAbility(StartSpec);
//			}
//
//			// 3. Activate Auto Active Abilities
//			const TArray<TSubclassOf<class UGPGameplayAbility_Base>> autoActiveAbilities = InAbilityAsset->GetAutoActiveAbilities();
//			for (const auto& Ability : autoActiveAbilities)
//			{
//				// Give Ability to ASC
//				FGameplayAbilitySpec StartSpec(Ability);
//				ASC->GiveAbility(StartSpec);
//
//				// Activate Ability
//				bool success = ASC->TryActivateAbility(StartSpec.Handle);
//				if (!success)
//				{
//					UE_LOG(LogTemplatePlayerCharacter, Warning, TEXT("Failed to activate Auto Active Skill."));
//				}
//			}
//
//			// 4. Apply Default Attribute Effect
//			const TSubclassOf<class UGameplayEffect> defaultAttributeEffect = InAbilityAsset->GetDefaultAttributesEffect();
//			if (IsValid(defaultAttributeEffect))
//			{
//				FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
//				EffectContext.AddSourceObject(this);
//				FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(defaultAttributeEffect, GetCharacterLevel(), EffectContext);
//				if (NewHandle.IsValid())
//				{
//					FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
//				}
//			}
//
//			// 5. Apply StartsUp Effect
//			const TArray<TSubclassOf<class UGameplayEffect>>& startsUpEffect = InAbilityAsset->GetStartupEffects();
//			for (const auto& effect : startsUpEffect)
//			{
//				FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
//				EffectContext.AddSourceObject(this);
//				FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(effect, GetCharacterLevel(), EffectContext);
//				if (NewHandle.IsValid())
//				{
//					FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
//				}
//			}
//		}
//	}
//}
//
//void AAbililtyCombatPlayerCharacter::RemoveGASAbilities(UGASInputDataAsset* InAbilityAsset)
//{
//	if (IsValid(ASC) && IsValid(InputComponent))
//	{
//		APlayerController* PlayerController = Cast<APlayerController>(Controller);
//		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
//		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
//
//		if (InAbilityAsset && InputSubsystem)
//		{
//			// Remove Mapping Context
//			InputSubsystem->RemoveMappingContext(InAbilityAsset->GetMappingContext());
//
//			const TSet<FGameplayInputAbilityInfo>& AbilitySet = InAbilityAsset->GetInputAbilities();
//			for (const auto& Ability : AbilitySet)
//			{
//				// Remove Ability From ASC
//				FGameplayAbilitySpec StartSpec(Ability.GameplayAbilityClass);
//				StartSpec.InputID = static_cast<int32>(Ability.InputID);
//				ASC->ClearAbility(StartSpec.Handle);
//
//				// Remove Enhanced Input Bind Or Add Binding Only Once
//				// EnhancedInputComponent->RemoveBinding()
//			}
//		}
//	}
//}

void AAbililtyCombatPlayerCharacter::BindASCInput()
{
	if (!ASCInputBound && IsValid(ASC) && IsValid(InputComponent))
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/AdvancedCombat"), FName("EACAbilityInputID"));
		ASC->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EACAbilityInputID::Confirm), static_cast<int32>(EACAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

void AAbililtyCombatPlayerCharacter::LevelUp_Callback(const FOnAttributeChangeData& AttributeChangeData)
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(ASC) || !LevelUpEffect)
	{
		return;
	}
}

void AAbililtyCombatPlayerCharacter::GASInputPressed(EACAbilityInputID InputId)
{
	UE_LOG(LogTemp, Display, TEXT("GASInputPressed"));

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(InputId));
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AAbililtyCombatPlayerCharacter::GASInputReleased(EACAbilityInputID InputId)
{
	UE_LOG(LogTemp, Display, TEXT("GASInputReleased"));

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(InputId));
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AAbililtyCombatPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (GetMovementComponent()->IsFlying())
		{
			const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

			if (Rotation.Pitch < 90)
				AddMovementInput(FVector(0, 0, 1), 1);
			else if (Rotation.Pitch > 270)
				AddMovementInput(FVector(0, 0, 1), -1);
		}
	}
}

void AAbililtyCombatPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAbililtyCombatPlayerCharacter::Interact()
{
	if (InventoryComp)
	{
		InventoryComp->Interact();
	}
}
