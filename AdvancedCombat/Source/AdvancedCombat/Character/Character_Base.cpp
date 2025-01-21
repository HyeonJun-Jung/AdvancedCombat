// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Base.h"
#include "InputActionValue.h"
#include "Kismet/KismetTextLibrary.h"
#include "Tag/ACGameplayTag.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Ability/ACAbilitySystemComponent.h"
#include "Ability/ACGameplayAbility_Base.h"
#include "Ability/ACGameplayEffect_Base.h"
#include "Ability/AttributeSets/ACAttributeSet_Base.h"

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

void ACharacter_Base::ShowDamage(float DamageAmount)
{
	//if (DamageNumberClass)
	//{
	//	UDamageWidgetComponent* DamageText = NewObject<UDamageWidgetComponent>(this, DamageNumberClass);
	//	DamageText->RegisterComponent();
	//	DamageText->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	//	//// Set Damage Text Color
	//	UWidget_DamageNumber* DamageWidget = Cast<UWidget_DamageNumber>(DamageText->GetWidget());
	//	if (IsValid(DamageWidget))
	//	{
	//		DamageWidget->SetDamage(DamageAmount);
	//	}
	//}
}

float ACharacter_Base::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Delegate_TakeDamage.Broadcast();

	return Damage;
}

void ACharacter_Base::Died()
{
	bIsAlive = false;
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

void ACharacter_Base::ShowHitReaction_Inplace(EACHitReactDirection hitDirection)
{
	if (!AnimInst) return;

	switch (hitDirection)
	{
	case EACHitReactDirection::None:
		break;
	case EACHitReactDirection::Left:
	{
		if (HitMontage_Left_Inplace)
			AnimInst->Montage_Play(HitMontage_Left_Inplace);
		break;
	}
	case EACHitReactDirection::Front:
	{
		if (HitMontage_Forward_Inplace)
			AnimInst->Montage_Play(HitMontage_Forward_Inplace);
		break;
	}
	case EACHitReactDirection::Right:
	{
		if (HitMontage_Right_Inplace)
			AnimInst->Montage_Play(HitMontage_Right_Inplace);
		break;
	}
	case EACHitReactDirection::Back:
	{
		if (HitMontage_Backward_Inplace)
			AnimInst->Montage_Play(HitMontage_Backward_Inplace);
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

UAbilitySystemComponent* ACharacter_Base::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACharacter_Base::SetHealth(float Health)
{
	if (IsValid(AttributeSetBase))
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void ACharacter_Base::SetMana(float Mana)
{
	if (IsValid(AttributeSetBase))
	{
		AttributeSetBase->SetMana(Mana);
	}
}

void ACharacter_Base::SetStamina(float Stamina)
{
	if (IsValid(AttributeSetBase))
	{
		AttributeSetBase->SetStamina(Stamina);
	}
}

int32 ACharacter_Base::GetCharacterLevel() const
{
	if (IsValid(AttributeSetBase))
	{
		AttributeSetBase->GetCharacterLevel();
	}
	return 0;
}

float ACharacter_Base::GetHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float ACharacter_Base::GetMaxHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float ACharacter_Base::GetMana() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMana();
	}
	return 0.0f;
}

float ACharacter_Base::GetMaxMana() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxMana();
	}
	return 0.0f;
}

float ACharacter_Base::GetStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStamina();
	}
	return 0.0f;
}

float ACharacter_Base::GetMaxStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxStamina();
	}
	return 0.0f;
}

float ACharacter_Base::GetMoveSpeed() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeed();
	}
	return 0.0f;
}

float ACharacter_Base::GetMoveSpeedBaseValue() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase.Get())->GetBaseValue();
	}
	return 0.0f;
}

void ACharacter_Base::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(ASC) || ASC->bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
		}
	}

	ASC->bStartupEffectsApplied = true;
}

void ACharacter_Base::InitializeAttributes()
{
	if (!IsValid(ASC))
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
	}
}

void ACharacter_Base::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(ASC) || ASC->bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
		}
	}

	ASC->bStartupEffectsApplied = true;
}


