// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipComponent.h"
#include "Character/Player/AbililtyCombatPlayerCharacter.h"
#include "ACEnums.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipComponent::EquipItem(FSlotStruct& InSlot, FItemStruct& InItem)
{
	AAbililtyCombatPlayerCharacter* Player = Cast<AAbililtyCombatPlayerCharacter>(GetOwner());
	if (!Player) return;

	if (InItem.EquipType == EEquipCategory::EEC_Weapon)
	{
		// Equip Weapon
		if (IsValid(InItem.SkeletonMesh))
		{
			Player->RightWeapon_Skeletal->SetSkeletalMesh(InItem.SkeletonMesh);
			Player->RightWeapon_Skeletal->AttachToComponent(Player->GetMesh(),
				FAttachmentTransformRules::KeepRelativeTransform, InItem.SocketName);
			Player->RightWeapon_Skeletal->SetRelativeTransform(InItem.RelativeTransform);

			Player->RightWeapon_Static->SetStaticMesh(nullptr);
		}
		else if (IsValid(InItem.StaticMesh))
		{
			Player->RightWeapon_Static->SetStaticMesh(InItem.StaticMesh);
			Player->RightWeapon_Static->AttachToComponent(Player->GetMesh(),
				FAttachmentTransformRules::KeepRelativeTransform, InItem.SocketName);
			Player->RightWeapon_Static->SetRelativeTransform(InItem.RelativeTransform);

			Player->RightWeapon_Skeletal->SetSkeletalMesh(nullptr);
		}

		// Set AnimInstance
		if (IsValid(InItem.WeaponAnimInstance))
		{
			if (IsValid(InItem.CharacterMesh))
				Player->GetMesh()->SetSkeletalMesh(InItem.CharacterMesh);
			Player->GetMesh()->SetAnimInstanceClass(InItem.WeaponAnimInstance);
		}

		// Set Weapon Type
		CurrentWeaponType = InItem.WeaponType;

		
		// Give Ability With Weapon (Attack Ability, Signature Ability)
		UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
		if (ASC)
		{
			if (InItem.AttackAbilityClass)
			{
				// Remove Before Ability
				ASC->ClearAllAbilitiesWithInputID(static_cast<int32>(EACAbilityInputID::Attack));

				// Give Abililty
				FGameplayAbilitySpec AbililtySpec(InItem.AttackAbilityClass);
				AbililtySpec.InputID = static_cast<int32>(EACAbilityInputID::Attack);
				ASC->GiveAbility(AbililtySpec);
			}

			if (InItem.SignatureAbilityClass)
			{
				// Remove Before Ability
				ASC->ClearAllAbilitiesWithInputID(static_cast<int32>(EACAbilityInputID::Signature));

				FGameplayAbilitySpec AbililtySpec(InItem.SignatureAbilityClass);
				AbililtySpec.InputID = static_cast<int32>(EACAbilityInputID::Signature);
				ASC->GiveAbility(AbililtySpec);
			}
		}
	}

}

