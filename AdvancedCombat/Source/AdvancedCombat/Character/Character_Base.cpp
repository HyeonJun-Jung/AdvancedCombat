// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Base.h"
#include "InputActionValue.h"
#include "Kismet/KismetTextLibrary.h"
#include "Tag/ACGameplayTag.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	AnimInst = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacter_Base::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Delegate_TakeDamage.Broadcast();

	return Damage;
}

EACHitReactDirection ACharacter_Base::GetHitReactDirection(const FVector& ImpactPoint, AActor* DamageCauser)
{
	if (!DamageCauser) return EACHitReactDirection::Front;

	const FVector& ActorLocation = GetActorLocation();
	const FVector& DamageCauseLocation = DamageCauser->GetActorLocation();
	FVector TowardVec = DamageCauseLocation - ActorLocation;
	float DotProductResult = FVector::DotProduct(GetActorForwardVector(), TowardVec);

	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DotProductResult > 0 || DistanceToRightLeftPlane >= 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Front"));
			return EACHitReactDirection::Front;
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Back"));
			return EACHitReactDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Right"));
			return EACHitReactDirection::Right;
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Left"));
			return EACHitReactDirection::Left;
		}
	}

	return EACHitReactDirection::Front;
}

EACHitReactDirection ACharacter_Base::GetRightOrLeft(const FVector& ImpactPoint, AActor* DamageCauser)
{
	const FVector& ActorLocation = GetActorLocation();
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());

	if (DistanceToFrontBackPlane >= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Right"));
		return EACHitReactDirection::Right;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Left"));
		return EACHitReactDirection::Left;
	}

	return EACHitReactDirection::Front;
}

void ACharacter_Base::ShowHitReaction(EACHitReactDirection hitDirection)
{
	if (!AnimInst) return;

	switch (hitDirection)
	{
	case EACHitReactDirection::None:
		break;
	case EACHitReactDirection::Left:
	{
		if (HitMontage_Left)
			AnimInst->Montage_Play(HitMontage_Left);
		break;
	}
	case EACHitReactDirection::Front:
	{
		if (HitMontage_Forward)
			AnimInst->Montage_Play(HitMontage_Forward);
		break;
	}
	case EACHitReactDirection::Right:
	{
		if (HitMontage_Right)
			AnimInst->Montage_Play(HitMontage_Right);
		break;
	}
	case EACHitReactDirection::Back:
	{
		if (HitMontage_Backward)
			AnimInst->Montage_Play(HitMontage_Backward);
		break;
	}
	default:
		break;
	}
}

void ACharacter_Base::ShowParriedReaction(EACHitReactDirection hitDirection)
{
	if (!AnimInst) return;

	switch (hitDirection)
	{
	case EACHitReactDirection::None:
		break;
	case EACHitReactDirection::Left:
	{
		if (ParriedMontage_Left)
			AnimInst->Montage_Play(ParriedMontage_Left);
		break;
	}
	case EACHitReactDirection::Front:
	{
		int randNum = UKismetMathLibrary::RandomIntegerInRange(0, 1);
		if (randNum == 0 && ParriedMontage_Left)
			AnimInst->Montage_Play(ParriedMontage_Left);
		else if (randNum == 1 && ParriedMontage_Right)
			AnimInst->Montage_Play(ParriedMontage_Right);
		break;
	}
	case EACHitReactDirection::Right:
	{
		if (ParriedMontage_Right)
			AnimInst->Montage_Play(ParriedMontage_Right);
		break;
	}
	case EACHitReactDirection::Back:
	{
		break;
	}
	default:
		break;
	}
}

