// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Samurai/Samurai.h"
#include "Character/Enemy/Samurai/Samurai_AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actor/ACProjectile.h"

void ASamurai::BeginPlay()
{
	Super::BeginPlay();

	SamuraiAnimInst = Cast<USamurai_AnimInstance>(GetMesh()->GetAnimInstance());
	if (SamuraiAnimInst)
	{
		SamuraiAnimInst->Delegate_SpawnSlash.AddUObject(this, &ASamurai::SpawnSlash);
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

void ASamurai::Dodge_Bwd()
{
	if (AnimInst && DodgeMontage_Bwd)
	{
		AnimInst->Montage_Play(DodgeMontage_Bwd);
	}
}
