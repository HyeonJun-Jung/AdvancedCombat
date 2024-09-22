// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/CombatPlayerCharacter.h"
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
#include "Kismet/KismetTextLibrary.h"
#include "Tag/ACGameplayTag.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/DamageType/DamageTypes.h"
#include "Character/Enemy/Samurai/Samurai.h"
#include "Component/LockOnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Player/State/Samurai/SamuraiState.h"

ACombatPlayerCharacter::ACombatPlayerCharacter()
{
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>("LockOnComponent");
}

void ACombatPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ACombatPlayerCharacter::Attack);
		}

		if (DefenseAction)
		{
			EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Triggered, this, &ACombatPlayerCharacter::DefenseStart);
			EnhancedInputComponent->BindAction(DefenseAction, ETriggerEvent::Completed, this, &ACombatPlayerCharacter::DefenseEnd);
		}

		if (DodgeAction)
		{
			EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ACombatPlayerCharacter::Dodge);
		}

		if (LockOnAction)
		{
			EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ACombatPlayerCharacter::LockOn);
		}
	}
}

void ACombatPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	CombatAnimInst = Cast<UCombatPlayer_AnimInstance>(GetMesh()->GetAnimInstance());
	LandedDelegate.AddUniqueDynamic(this, &ACombatPlayerCharacter::Landed);

	// Combat State
	if (CombatStateClass_Samurai)
		SamuraiState = NewObject<UCombatState>(this, CombatStateClass_Samurai, "Samurai Combat State");
	else
		SamuraiState = NewObject<UCombatState>();

	CombatState = SamuraiState;
	CombatState->EnterState(this);

}

void ACombatPlayerCharacter::DisableMovementInput(float Duration)
{
	bMoveable = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DisableMovement,
		[&]()
		{
			bMoveable = true;
		}
	, Duration, false);
}

bool ACombatPlayerCharacter::IsJumpable()
{
	return !TagContainer.HasTag(TAG_CHARACTER_STATE_ATTACKING) 
		&& !TagContainer.HasTag(TAG_CHARACTER_STATE_DAMAGED);
}

void ACombatPlayerCharacter::Jump()
{
	if (!IsJumpable()) return;

	if (CombatAnimInst)
		CombatAnimInst->SetDefensing(false);

	Super::Jump();
	TagContainer.AddTag(TAG_CHARACTER_STATE_JUMPING);
}

void ACombatPlayerCharacter::StopJumping()
{
	Super::StopJumping();
}

void ACombatPlayerCharacter::Landed(const FHitResult& hit)
{
	TagContainer.RemoveTag(TAG_CHARACTER_STATE_JUMPING);
}

void ACombatPlayerCharacter::LockOn()
{
	if (!LockOnComponent) return;
	LockOnComponent->LockOn();
}

bool ACombatPlayerCharacter::IsDodgeable()
{
	return !TagContainer.HasTag(TAG_CHARACTER_STATE_DODGING)
		&& !TagContainer.HasTag(TAG_CHARACTER_STATE_ATTACKING)
		&& !TagContainer.HasTag(TAG_CHARACTER_STATE_DAMAGED)
		&& !TagContainer.HasTag(TAG_CHARACTER_STATE_JUMPING);
}

void ACombatPlayerCharacter::Dodge()
{
	if (!CombatState) return;
	CombatState->Dodge();
}

void ACombatPlayerCharacter::Attack()
{
	if (!CombatState) return;
	CombatState->Attack();
}

void ACombatPlayerCharacter::DefenseStart()
{
	if (!CombatState) return;
	CombatState->DefenseStart();
}

void ACombatPlayerCharacter::DefenseEnd()
{
	if (!CombatState) return;
	CombatState->DefenseEnd();
}

float ACombatPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (!CombatState) return damage;
	damage = CombatState->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return damage;
}
