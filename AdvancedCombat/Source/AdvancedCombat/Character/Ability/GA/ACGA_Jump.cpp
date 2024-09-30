// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/ACGA_Jump.h"
#include "GameFramework/Character.h"
#include "Character/Ability/GA/AT/ACAT_JumpAndWaitForLanding.h"

UACGA_Jump::UACGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UACGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags);
	if (!bResult)
		return false;

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

	return Character && Character->CanJump();
}

void UACGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UACAT_JumpAndWaitForLanding* JumpTask = UACAT_JumpAndWaitForLanding::CreateTask(this);
	JumpTask->OnComplete.AddDynamic(this, &UACGA_Jump::OnLandedCallback);
	JumpTask->ReadyForActivation();
}

void UACGA_Jump::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UACGA_Jump::OnLandedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}