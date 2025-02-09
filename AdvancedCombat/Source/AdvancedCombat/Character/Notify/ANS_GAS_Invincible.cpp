// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_GAS_Invincible.h"
#include "Character/Character_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "ACGameplayTags.h"

void UANS_GAS_Invincible::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	character = Cast<ACharacter_Base>(MeshComp->GetOwner());
	if (!IsValid(character)) return;
	UAbilitySystemComponent* ASC = character->GetAbilitySystemComponent();
	if (ASC)
	{
		ASC->AddLooseGameplayTag(FACGameplayTags::Get().Character_State_Invincible);
	}
}

void UANS_GAS_Invincible::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (!IsValid(character)) return; 
	UAbilitySystemComponent* ASC = character->GetAbilitySystemComponent();
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(FACGameplayTags::Get().Character_State_Invincible);
	}
}

