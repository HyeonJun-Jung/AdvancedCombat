// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SK_Magician/SK_Magician.h"
#include "SKM_AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/ACProjectile.h"

void ASK_Magician::BeginPlay()
{
	Super::BeginPlay();

	SKM_AnimInst = Cast<USKM_AnimInstance>(GetMesh()->GetAnimInstance());
	if (SKM_AnimInst)
	{
		SKM_AnimInst->Delegate_SpawnMagic_Hand.AddUObject(this, &ASK_Magician::SpawnMagic_Hand);
		SKM_AnimInst->Delegate_SpawnMagic_Staff.AddUObject(this, &ASK_Magician::SpawnMagic_Staff);
		SKM_AnimInst->Delegate_Teleport.AddUObject(this, &ASK_Magician::Teleport_Callback);
	}
}

void ASK_Magician::MagicAttack(ESKM_MagicType InMagicType)
{
	if (!SKM_AnimInst) return;

	CurrentMagicType = InMagicType;

	switch (InMagicType)
	{
	case ESKM_MagicType::FireBall:
	{
		SKM_AnimInst->Montage_Play(AttackMontage_01);
		break;
	}
	case ESKM_MagicType::FireArrow: 
	{
		SKM_AnimInst->Montage_Play(AttackMontage_01_Arranged);
		break;
	}
	default:
		break;
	}
}

void ASK_Magician::Teleport()
{
	if (!SKM_AnimInst)return;
	SKM_AnimInst->Montage_Play(TeleportMontage);
}

void ASK_Magician::SpawnMagic_Hand()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Magic_Hand"));
	SpawnMagic(SpawnLocation);
	
}

void ASK_Magician::SpawnMagic_Staff()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Magic_Staff"));
	SpawnMagic(SpawnLocation);
}

void ASK_Magician::SpawnMagic(FVector& SpawnLocation)
{
	switch (CurrentMagicType)
	{
	case ESKM_MagicType::FireBall:
	{
		if (!FireBall_C) return;

		AACProjectile* magic = GetWorld()->SpawnActor<AACProjectile>(FireBall_C, SpawnLocation, FRotator::ZeroRotator);
		if (magic)
		{
			ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
			if (!player) return;

			FVector towardVec = player->GetActorLocation() - SpawnLocation;
			towardVec.Normalize();

			magic->AddForceToProjectile(towardVec * 200);
		}
		break;
	}
	case ESKM_MagicType::FireArrow:
	{
		if (!FireArrow_C) return;

		AACProjectile* magic = GetWorld()->SpawnActor<AACProjectile>(FireArrow_C, SpawnLocation, FRotator::ZeroRotator);
		if (magic)
		{
			ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
			if (!player) return;

			FVector towardVec = player->GetActorLocation() - SpawnLocation;
			towardVec.Normalize();

			magic->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnLocation, player->GetActorLocation()));
			magic->AddForceToProjectile(towardVec * 250);
		}
		break;
	}
	default:
		break;
	}
}

void ASK_Magician::Teleport_Callback()
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!player) return;
	
	FVector destination = GetActorLocation();
	int tried = 0;
	while (tried <= 10)
	{
		FVector RandomPoint = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * 1500;
		
		FHitResult hitResult;
		GetWorld()->LineTraceSingleByChannel(hitResult, RandomPoint + FVector(0, 0, 500), RandomPoint + FVector(0, 0, -500),
			ECollisionChannel::ECC_Visibility);
		
		if (hitResult.bBlockingHit)
		{
			destination = hitResult.ImpactPoint;
			break;
		}

		tried++;
	}
	 
	SetActorLocation(destination);
}
