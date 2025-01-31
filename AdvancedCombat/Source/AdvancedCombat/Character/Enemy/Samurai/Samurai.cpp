// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Samurai/Samurai.h"
#include "Character/Enemy/Samurai/Samurai_AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/ACProjectile.h"
#include "Character/Player/CombatPlayerCharacter.h"
#include "Engine/DamageEvents.h"
#include "Character/DamageType/DamageTypes.h"

void ASamurai::BeginPlay()
{
	Super::BeginPlay();

	SamuraiAnimInst = Cast<USamurai_AnimInstance>(GetMesh()->GetAnimInstance());
	if (SamuraiAnimInst)
	{
		SamuraiAnimInst->Delegate_SpawnSlash.AddUObject(this, &ASamurai::SpawnSlash);
		SamuraiAnimInst->Delegate_SpawnIdleSlash.AddUObject(this, &ASamurai::SpawnIdleSlash);
	}
}

void ASamurai::NormalAttack_Left()
{
	if (!AnimInst || !Montage_NormalAttack_Left) return;
	AnimInst->Montage_Play(Montage_NormalAttack_Left);
}

void ASamurai::NormalAttack_Right()
{
	if (!AnimInst || !Montage_NormalAttack_Right) return;
	AnimInst->Montage_Play(Montage_NormalAttack_Right);
}

void ASamurai::RandomComboAttack()
{
	int rand = UKismetMathLibrary::RandomIntegerInRange(1, 3);
	switch (rand)
	{
	case 1:
	{
		if (!AnimInst || !Montage_AttackCombo01) return;
		AnimInst->Montage_Play(Montage_AttackCombo01);
		break;
	}
	case 2:
	{
		if (!AnimInst || !Montage_AttackCombo02) return;
		AnimInst->Montage_Play(Montage_AttackCombo02);
		break;
	}
	case 3:
	{
		if (!AnimInst || !Montage_AttackCombo03) return;
		AnimInst->Montage_Play(Montage_AttackCombo03);
		break;
	}
	default:
		break;
	}
}

void ASamurai::ComboAttack(int ComboIdx)
{
	if (ComboIdx == 0)
	{
		ComboIdx = UKismetMathLibrary::RandomIntegerInRange(1, 3);
	}

	switch (ComboIdx)
	{
	case 1:
	{
		if (!AnimInst || !Montage_AttackCombo01) return;
		AnimInst->Montage_Play(Montage_AttackCombo01);
		break;
	}
	case 2:
	{
		if (!AnimInst || !Montage_AttackCombo02) return;
		AnimInst->Montage_Play(Montage_AttackCombo02);
		break;
	}
	case 3:
	{
		if (!AnimInst || !Montage_AttackCombo03) return;
		AnimInst->Montage_Play(Montage_AttackCombo03);
		break;
	}
	default:
		break;
	}
}

void ASamurai::SpecialAttack(int AttackIdx)
{
	switch (AttackIdx)
	{
	case 1:
	{
		if (!AnimInst || !Montage_SPAttack01) return;
		AnimInst->Montage_Play(Montage_SPAttack01);
		break;
	}
	case 2:
	{
		if (!AnimInst || !Montage_SPAttack02) return;
		AnimInst->Montage_Play(Montage_SPAttack02);
		break;
	}
	case 3:
	{
		if (!AnimInst || !Montage_SPAttack03) return;
		AnimInst->Montage_Play(Montage_SPAttack03);
		break;
	}
	case 4:
	{
		if (!AnimInst || !Montage_SPAttack04) return;
		AnimInst->Montage_Play(Montage_SPAttack04);
		break;
	}
	case 5:
	{
		if (!AnimInst || !Montage_SPAttack05) return;
		AnimInst->Montage_Play(Montage_SPAttack05);
		break;
	}
	default:
		break;
	}
}

void ASamurai::JumpAttack()
{
	if (AnimInst && Montage_JumpAttack)
	{
		AnimInst->Montage_Play(Montage_JumpAttack);
	}
}

void ASamurai::StingFailed()
{
	if (AnimInst)
		AnimInst->Montage_JumpToSection(FName("Failed"));
}

void ASamurai::SpawnSlash()
{
	if (!SlashActorClass) return;

	FVector Location_Katana = GetMesh()->GetSocketLocation(FName("Weapon_Katana"));
	FRotator Rotation_Katana = GetMesh()->GetSocketRotation(FName("Weapon_Katana"));
	FRotator Rotation_Slash = FRotator(0, 0, Rotation_Katana.Roll);

	AACProjectile* slash = GetWorld()->SpawnActor<AACProjectile>(SlashActorClass,
		Location_Katana, Rotation_Slash + GetActorRotation());
	if (slash)
	{
		slash->AddForceToProjectile(GetActorForwardVector());
	}
}

void ASamurai::SpawnIdleSlash()
{
	if (IdleSlashActorClass)
	{
		FVector Location_Katana = GetMesh()->GetSocketLocation(FName("Weapon_Katana"));
		FRotator Rotation_Katana = GetMesh()->GetSocketRotation(FName("Weapon_Katana"));
		FRotator Rotation_Slash = FRotator(0, 0, Rotation_Katana.Roll + Slash_AdditionalRoll);

		GetWorld()->SpawnActor<AActor>(IdleSlashActorClass,
			Location_Katana, Rotation_Slash + GetActorRotation());
	}
}

void ASamurai::Dodge_Bwd()
{
	if (AnimInst && DodgeMontage_Bwd)
	{
		AnimInst->Montage_Play(DodgeMontage_Bwd);
	}
}

bool ASamurai::IsParryable()
{
	if(AnimInst->GetCurrentActiveMontage())
		return false;
	return true;
}

float ASamurai::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

void ASamurai::DealWithDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

	EACHitReactDirection hitDirection = GetHitReactDirection(hitResult.ImpactPoint, DamageCauser);
	EACHitReactDirection hitDir_RightLeft = GetRightOrLeft(hitResult.ImpactPoint, DamageCauser);

	if (IsParryable())
	{
		if (DamageEvent.DamageTypeClass == UGuardableDamage::StaticClass())
		{
			if (hitDir_RightLeft == EACHitReactDirection::Right)
			{
				AnimInst->Montage_Play(GuardSuccessMontage_Right);
			}
			else
			{
				AnimInst->Montage_Play(GuardSuccessMontage_Left);
			}
			LaunchCharacter(GetActorForwardVector() * -750, false, false);
		}
		else if (DamageEvent.DamageTypeClass == UParryableDamage::StaticClass())
		{
			ACombatPlayerCharacter* CombatCharacter = Cast<ACombatPlayerCharacter>(DamageCauser);
			if (!CombatCharacter) return;

			CombatCharacter->Delegate_Parried.Broadcast(hitDirection);
			CombatCharacter->ShowParriedReaction(hitDirection);

			AnimInst->Montage_Play(ParryMontage);
		}
	}
}
