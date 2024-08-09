// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Samurai/Samurai.h"
#include "Character/Enemy/Samurai/Samurai_AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void ASamurai::BeginPlay()
{
	Super::BeginPlay();

	AnimInst = Cast<USamurai_AnimInstance>(GetMesh()->GetAnimInstance());
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
	}
	case 2:
	{
		if (!AnimInst || !Montage_AttackCombo02) return;
		AnimInst->Montage_Play(Montage_AttackCombo02);
	}
	case 3:
	{
		if (!AnimInst || !Montage_AttackCombo03) return;
		AnimInst->Montage_Play(Montage_AttackCombo03);
	}
	default:
		break;
	}
}
