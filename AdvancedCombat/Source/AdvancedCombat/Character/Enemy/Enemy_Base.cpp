// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Enemy_Base.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Ability/ACAbilitySystemComponent.h"
#include "Character/Ability/AttributeSets/ACAttributeSet_Base.h"
#include "Character/Ability/ACGameplayAbility_Base.h"
#include "Character/Ability/ACGameplayEffect_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Character/DamageType/DamageTypes.h"
#include "Actor/PatrolRoute.h"

AEnemy_Base::AEnemy_Base()
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

	// Ability System
	ASC = CreateDefaultSubobject<UACAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSetBase = CreateDefaultSubobject<UACAttributeSet_Base>(TEXT("AttributeSet"));

	// Status Widget Component
	/*UIFloatingStatusBarComponent = CreateDefaultSubobject<UFloatingStatusBarComponent>(FName("UIFloatingStatusBarComponent"));
	UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
	UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0, 0, 120));
	UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500, 500));

	UIFloatingStatusBarClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/GAS_Project/DefenseGame/Widget/WBP_FloatingStatusBar_Monster.WBP_FloatingStatusBar_Monster_C"));
	if (!UIFloatingStatusBarClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find UIFloatingStatusBarClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}*/
}

void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);

		InitializeAttributes();

		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());

		AddStartupEffects();
		AddCharacterAbilities();

		// Setup FloatingStatusBar UI for Locally Owned Players only, not AI or the server's copy of the PlayerControllers
		//APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//if (PC && PC->IsLocalPlayerController())
		//{
		//	if (UIFloatingStatusBarClass)
		//	{
		//		UIFloatingStatusBar = CreateWidget<UWidget_FloatingStatusBar>(PC, UIFloatingStatusBarClass);
		//		if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
		//		{
		//			UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);

		//			// Setup the floating status bar
		//			UIFloatingStatusBar->SetHealthPercentage(GetHealth() / GetMaxHealth());

		//			UIFloatingStatusBar->SetCharacterName(CharacterName);
		//		}
		//	}
		//}

		// Attribute change callbacks
		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AEnemy_Base::HealthChanged);

		// Tag change callbacks
		// ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGPMinion::StunTagChanged);
	}
}

void AEnemy_Base::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Display, TEXT("AGPMinion : Health Changed."));

	float Health = Data.NewValue;

	// Update floating status bar
	/*if (UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
	}*/

	// If the minion died, handle death
	/*if (!IsAlive() && !ASC->HasMatchingGameplayTag(DeadTag))
	{
		Die();
	}*/
}

float AEnemy_Base::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (damage > 0)
	{
		DealWithDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	}
	
	return damage;
}

void AEnemy_Base::DealWithDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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


	// Air Combo
	if (DamageEvent.DamageTypeClass == UAirborneDamage::StaticClass())
	{
		LaunchCharacter(FVector(0, 0, 2500), false, false);
		LimitMaxHeight(GetActorLocation().Z + 750.f);
		ShowHitReaction_Inplace(hitDirection);
	}
	else if (DamageEvent.DamageTypeClass == UInAirDamage::StaticClass())
	{
		GetCharacterMovement()->Velocity.Z = 0;
		DisableGravity(0.2f);
		ShowHitReaction_Inplace(hitDirection);
	}
	else if (DamageEvent.DamageTypeClass == UHitDownDamage::StaticClass())
	{
		GetCharacterMovement()->GravityScale = 1.f;
		LaunchCharacter(FVector(0, 0, -1250), false, false);

		if (HitMontage_HitDown_Inplace)
			AnimInst->Montage_Play(HitMontage_HitDown_Inplace);

		LandedDelegate.AddDynamic(this, &AEnemy_Base::GetUpFromGround);
	}
	else
	{
		ShowHitReaction(hitDirection);
	}
}

void AEnemy_Base::LimitMaxHeight(float InMaxHeight)
{
	maxHeight = InMaxHeight;
	GetWorld()->GetTimerManager().SetTimer(handle_LimitMaxHeight,
		[&]()
		{
			if (GetActorLocation().Z >= maxHeight)
			{
				GetCharacterMovement()->Velocity.Z = 0;
				GetWorld()->GetTimerManager().ClearTimer(handle_LimitMaxHeight);
			}
		}, 0.01f, true);
}

void AEnemy_Base::DisableGravity(float duration)
{
	GetCharacterMovement()->GravityScale = 0.f;

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,
		[&]()
		{
			GetCharacterMovement()->GravityScale = 1.f;
		},
		duration, false);
}

void AEnemy_Base::GetUpFromGround(const FHitResult& hit)
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,
		[&]()
		{
			if (HitMontage_GetUp)
				AnimInst->Montage_Play(HitMontage_GetUp);
			LandedDelegate.RemoveDynamic(this, &AEnemy_Base::GetUpFromGround);
		},
		0.2f, false);
}
