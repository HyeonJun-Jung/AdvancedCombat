// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/State/Samurai/SamuraiState.h"
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

USamuraiState::USamuraiState()
{
}

USamuraiState::~USamuraiState()
{
}

void USamuraiState::EnterState(ACombatPlayerCharacter* InCombatCharacter)
{
	Super::EnterState(InCombatCharacter);
}

void USamuraiState::ExitState()
{
}

void USamuraiState::SetUpDelegate()
{
}

bool USamuraiState::IsDodgeable()
{
	return !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_DODGING)
		&& !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_ATTACKING)
		&& !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_DAMAGED)
		&& !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_JUMPING);
}

void USamuraiState::Dodge()
{
	if (!IsDodgeable()) return;

	if (!PlayerController || !AnimInst) return;

	CombatCharacter->TagContainer.AddTag(TAG_CHARACTER_STATE_DODGING);

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

bool USamuraiState::IsAttackable()
{
	return !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_JUMPING)
		&& !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_DAMAGED);
}

void USamuraiState::Attack()
{
	if (CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_DODGING))
	{
		bShouldDoDashAttack = true;
		return;
	}

	if (!IsAttackable())
		return;

	if (CurrentCombo == 0 && AnimInst->GetCurrentActiveMontage() == nullptr)
	{
		CombatCharacter->TagContainer.AddTag(TAG_CHARACTER_STATE_ATTACKING);

		ResetCombo();

		if (AnimInst && AttackMontage_01)
		{
			AnimInst->Montage_Play(AttackMontage_01);
			CurrentCombo++;
		}
	}
	else if (bCanAttackInput)
	{
		bShouldDoNextAttack = true;
		bCanAttackInput = false;
	}
}

void USamuraiState::DashAttack()
{
	if (CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_DODGING) && bShouldDoDashAttack)
	{
		CombatCharacter->TagContainer.RemoveTag(TAG_CHARACTER_STATE_DODGING);
		CombatCharacter->TagContainer.AddTag(TAG_CHARACTER_STATE_ATTACKING);
		bShouldDoDashAttack = false;
		AnimInst->Montage_Play(Montage_DodgeAttack);
		return;
	}
}

void USamuraiState::ResetCombo()
{
	CurrentCombo = 0;
	bShouldDoNextAttack = false;
	bCanAttackInput = false;
	bNextAttackChecking = false;
}

void USamuraiState::AttackInputStart()
{
	bCanAttackInput = true;
}

void USamuraiState::CheckShouldAttack()
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
		AnimInst->Montage_Play(AttackMontage_01);
		AnimInst->Montage_JumpToSection(NextSectionName);
	}
}

bool USamuraiState::IsDefenseAble()
{
	return !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_JUMPING)
		&& !CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_ATTACKING);
}

void USamuraiState::DefenseStart()
{
	if (AnimInst && IsDefenseAble())
	{
		if (CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_DAMAGED))
		{
			CombatCharacter->TagContainer.RemoveTag(TAG_CHARACTER_STATE_DAMAGED);
			AnimInst->Montage_Stop(0.1);
		}

		// Defense
		AnimInst->SetDefensing(true);
		CombatCharacter->TagContainer.AddTag(TAG_CHARACTER_STATE_DEFENSING);

		// Parry
		if (!TimerHandle_Parryable.IsValid())
		{
			CombatCharacter->TagContainer.AddTag(TAG_CHARACTER_STATE_PARRYABLE);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Parryable,
				[&]()
				{
					UE_LOG(LogTemp, Display, TEXT("Parry End."));
					CombatCharacter->TagContainer.RemoveTag(TAG_CHARACTER_STATE_PARRYABLE);
				},
				ParryTime, false);
		}
	}
}

void USamuraiState::DefenseEnd()
{
	if (AnimInst)
	{
		// Defense
		AnimInst->SetDefensing(false);

		// Parry
		CombatCharacter->TagContainer.RemoveTag(TAG_CHARACTER_STATE_DEFENSING);
		TimerHandle_Parryable.Invalidate();
	}
}

float USamuraiState::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (CombatCharacter->bInvincible)
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

void USamuraiState::DealWithSamurai(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, ASamurai* Samurai)
{
	FHitResult hitResult;
	FVector ImpulseDir;

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		PointDamageEvent->GetBestHitInfo(CombatCharacter, EventInstigator, hitResult, ImpulseDir);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
		RadialDamageEvent->GetBestHitInfo(CombatCharacter, EventInstigator, hitResult, ImpulseDir);
	}
	else
	{
		DamageEvent.GetBestHitInfo(CombatCharacter, EventInstigator, hitResult, ImpulseDir);
	}

	EACHitReactDirection hitDirection = CombatCharacter->GetHitReactDirection(hitResult.ImpactPoint, Samurai);
	EACHitReactDirection hitDir_RightLeft = CombatCharacter->GetRightOrLeft(hitResult.ImpactPoint, Samurai);

	if (hitDirection != EACHitReactDirection::Back
		&& CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_DEFENSING))
	{
		if (DamageEvent.DamageTypeClass == UParryableDamage::StaticClass()
			&& CombatCharacter->TagContainer.HasTag(TAG_CHARACTER_STATE_PARRYABLE))
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
				CombatCharacter->DisableMovementInput(0.5f);
				// CombatAnimInst->SetParried(true, EParryType::RightToLeft);
				AnimInst->Montage_Play(GuardSuccessMontage_Right);
				CombatCharacter->LaunchCharacter(CombatCharacter->GetActorForwardVector() * -1000, false, false);
			}
			else
			{
				CombatCharacter->DisableMovementInput(0.5f);
				// CombatAnimInst->SetParried(true, EParryType::LeftToRight);
				AnimInst->Montage_Play(GuardSuccessMontage_Left);
				CombatCharacter->LaunchCharacter(CombatCharacter->GetActorForwardVector() * -1000, false, false);
			}
		}
		else if (DamageEvent.DamageTypeClass == UStingDamage::StaticClass())
		{
			AnimInst->SetDefensing(false);
			CombatCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
			CombatCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			CombatCharacter->bUseControllerRotationYaw = false;

			Samurai->StingFailed();
			AnimInst->Montage_Play(StingParryMontage);
		}
	}
	else
	{
		CombatCharacter->TagContainer.AddTag(TAG_CHARACTER_STATE_DAMAGED);
		CombatCharacter->ShowHitReaction(hitDirection);
	}
}
