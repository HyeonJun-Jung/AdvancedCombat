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
	/*if(PlayerController)
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));*/

	CombatAnimInst = Cast<UCombatPlayer_AnimInstance>(GetMesh()->GetAnimInstance());
	if (!CombatAnimInst)
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to get UCombatPlayer_AnimInstance."), *GetNameSafe(this));
	}
	else
	{
		CombatAnimInst->OnMontageEnded.AddUniqueDynamic(this, &ACombatPlayerCharacter::MontageEnded);
		CombatAnimInst->Delegate_AttackInputStart.AddUObject(this, &ACombatPlayerCharacter::AttackInputStart);
		CombatAnimInst->Delegate_CheckDoNextAttack.AddUObject(this, &ACombatPlayerCharacter::CheckShouldAttack);
		CombatAnimInst->Delegate_DashAttack .AddUObject(this, &ACombatPlayerCharacter::DashAttack);
	}

	LandedDelegate.AddUniqueDynamic(this, &ACombatPlayerCharacter::Landed);
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

void ACombatPlayerCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	if (CurrentCombo == 0)
		TagContainer.RemoveTag(TAG_CHARACTER_STATE_ATTACKING);

	/*if (TagContainer.HasTag(TAG_CHARACTER_STATE_DODGING) && bShouldDoDodgeAttack)
	{
		TagContainer.RemoveTag(TAG_CHARACTER_STATE_DODGING);
		TagContainer.AddTag(TAG_CHARACTER_STATE_ATTACKING);
		bShouldDoDodgeAttack = false;
		AnimInst->Montage_Play(Montage_DodgeAttack);
		return;
	}*/

	if (!bInterrupted)
	{
		TagContainer.RemoveTag(TAG_CHARACTER_STATE_ATTACKING);
		TagContainer.RemoveTag(TAG_CHARACTER_STATE_DAMAGED);
		TagContainer.RemoveTag(TAG_CHARACTER_STATE_DODGING);
		ResetCombo();
	}
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
	if (!IsDodgeable()) return;

	if (!PlayerController || !AnimInst) return;

	TagContainer.AddTag(TAG_CHARACTER_STATE_DODGING);
	
	if (PlayerController->IsInputKeyDown(EKeys::A) && !PlayerController->IsInputKeyDown(EKeys::D))
	{
		AnimInst->Montage_Play(DodgeMontage_Left);
	}
	else if (PlayerController->IsInputKeyDown(EKeys::D) && !PlayerController->IsInputKeyDown(EKeys::A))
	{
		AnimInst->Montage_Play(DodgeMontage_Right);
	}
	else if (PlayerController->IsInputKeyDown(EKeys::W) && !PlayerController->IsInputKeyDown(EKeys::S))
	{
		AnimInst->Montage_Play(DodgeMontage_Fwd);
	}
	else
	{
		AnimInst->Montage_Play(DodgeMontage_Bwd);
	}
}

bool ACombatPlayerCharacter::IsAttackable()
{
	return !TagContainer.HasTag(TAG_CHARACTER_STATE_JUMPING)
		&& !TagContainer.HasTag(TAG_CHARACTER_STATE_DAMAGED);
}

void ACombatPlayerCharacter::Attack()
{
	if (TagContainer.HasTag(TAG_CHARACTER_STATE_DODGING))
	{
		bShouldDoDashAttack = true;
		return;
	}

	if (!IsAttackable())
		return;

	if (CurrentCombo == 0 && CombatAnimInst->GetCurrentActiveMontage() == nullptr)
	{
		TagContainer.AddTag(TAG_CHARACTER_STATE_ATTACKING);

		ResetCombo();

		if (CombatAnimInst && AttackMontage_01)
		{
			CombatAnimInst->Montage_Play(AttackMontage_01);
			CurrentCombo++;
		}
	}
	else if(bCanAttackInput)
	{
		bShouldDoNextAttack = true;
		bCanAttackInput = false;
	}
}

void ACombatPlayerCharacter::DashAttack()
{
	if (TagContainer.HasTag(TAG_CHARACTER_STATE_DODGING) && bShouldDoDashAttack)
	{
		TagContainer.RemoveTag(TAG_CHARACTER_STATE_DODGING);
		TagContainer.AddTag(TAG_CHARACTER_STATE_ATTACKING);
		bShouldDoDashAttack = false;
		AnimInst->Montage_Play(Montage_DodgeAttack);
		return;
	}
}

void ACombatPlayerCharacter::AttackInputStart()
{
	bCanAttackInput = true;
}

void ACombatPlayerCharacter::CheckShouldAttack()
{
	if (CurrentCombo >= MaxCombo || !bShouldDoNextAttack)
	{
		ResetCombo();
		return;
	}

	if (bShouldDoNextAttack)
	{
		bShouldDoNextAttack = false;
		FName NextSectionName = FName(FString::FromInt(++CurrentCombo));
		CombatAnimInst->Montage_Play(AttackMontage_01);
		CombatAnimInst->Montage_JumpToSection(NextSectionName);
	}
}

void ACombatPlayerCharacter::ResetCombo()
{
	CurrentCombo = 0;
	bShouldDoNextAttack = false;
	bCanAttackInput = false;
	bNextAttackChecking = false;
}

bool ACombatPlayerCharacter::IsDefenseAble()
{
	return !TagContainer.HasTag(TAG_CHARACTER_STATE_JUMPING)
		&& !TagContainer.HasTag(TAG_CHARACTER_STATE_ATTACKING);
}

void ACombatPlayerCharacter::DefenseStart()
{
	if (CombatAnimInst && IsDefenseAble())
	{
		if (TagContainer.HasTag(TAG_CHARACTER_STATE_DAMAGED))
		{
			TagContainer.RemoveTag(TAG_CHARACTER_STATE_DAMAGED);
			AnimInst->Montage_Stop(0.1);
		}

		// Defense
		CombatAnimInst->SetDefensing(true);
		TagContainer.AddTag(TAG_CHARACTER_STATE_DEFENSING);

		// Parry
		if (!TimerHandle_Parryable.IsValid())
		{
			TagContainer.AddTag(TAG_CHARACTER_STATE_PARRYABLE);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Parryable,
				[&]()
				{
					UE_LOG(LogTemp, Display, TEXT("Parry End."));
					TagContainer.RemoveTag(TAG_CHARACTER_STATE_PARRYABLE);
				},
				ParryTime, false);
		}
	}
}

void ACombatPlayerCharacter::DefenseEnd()
{
	if (CombatAnimInst)
	{
		// Defense
		CombatAnimInst->SetDefensing(false);

		// Parry
		TagContainer.RemoveTag(TAG_CHARACTER_STATE_DEFENSING);
		TimerHandle_Parryable.Invalidate();
	}
}

float ACombatPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (bInvincible) 
		return damage;

	ResetCombo();

	ASamurai* samurai = Cast<ASamurai>(DamageCauser);
	if (samurai)
	{
		DealWithSamurai(Damage, DamageEvent, EventInstigator, samurai);
		return damage;
	}

	return damage;
}

void ACombatPlayerCharacter::DealWithSamurai(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, ASamurai* Samurai)
{
	FHitResult hitResult;
	FVector ImpulseDir;

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		PointDamageEvent->GetBestHitInfo(this, EventInstigator, hitResult, ImpulseDir);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
		RadialDamageEvent->GetBestHitInfo(this, EventInstigator, hitResult, ImpulseDir);
	}
	else
	{
		DamageEvent.GetBestHitInfo(this, EventInstigator, hitResult, ImpulseDir);
	}

	EACHitReactDirection hitDirection = GetHitReactDirection(hitResult.ImpactPoint, Samurai);
	EACHitReactDirection hitDir_RightLeft = GetRightOrLeft(hitResult.ImpactPoint, Samurai);

	if (hitDirection != EACHitReactDirection::Back
		&& TagContainer.HasTag(TAG_CHARACTER_STATE_DEFENSING))
	{
		if (DamageEvent.DamageTypeClass == UParryableDamage::StaticClass()
			&& TagContainer.HasTag(TAG_CHARACTER_STATE_PARRYABLE))
		{
			Samurai->Delegate_Parried.Broadcast();
			Samurai->ShowParriedReaction(hitDirection);

			AnimInst->Montage_Play(ParryMontage);
		}
		else if (DamageEvent.DamageTypeClass == UParryableDamage::StaticClass()
			|| DamageEvent.DamageTypeClass == UGuardableDamage::StaticClass())
		{
			if (hitDir_RightLeft == EACHitReactDirection::Right)
			{
				DisableMovementInput(0.5f);
				// CombatAnimInst->SetParried(true, EParryType::RightToLeft);
				AnimInst->Montage_Play(GuardSuccessMontage_Right);
				LaunchCharacter(GetActorForwardVector() * -1000, false, false);
			}
			else
			{
				DisableMovementInput(0.5f);
				// CombatAnimInst->SetParried(true, EParryType::LeftToRight);
				AnimInst->Montage_Play(GuardSuccessMontage_Left);
				LaunchCharacter(GetActorForwardVector() * -1000, false, false);
			}
		}
		else if (DamageEvent.DamageTypeClass == UStingDamage::StaticClass())
		{
			CombatAnimInst->SetDefensing(false);
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;

			Samurai->StingFailed();
			AnimInst->Montage_Play(StingParryMontage);
		}
	}
	else
	{
		TagContainer.AddTag(TAG_CHARACTER_STATE_DAMAGED);
		ShowHitReaction(hitDirection);
	}
}

void ACombatPlayerCharacter::EnableInvincible(float Duration)
{
	bInvincible = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Invincible,
		[&]()
		{
			bInvincible = false;
		}
	, Duration, false);
}
