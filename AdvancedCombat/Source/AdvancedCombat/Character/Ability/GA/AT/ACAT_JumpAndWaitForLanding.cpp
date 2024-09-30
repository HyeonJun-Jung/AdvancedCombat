// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/AT/ACAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UACAT_JumpAndWaitForLanding::UACAT_JumpAndWaitForLanding()
{
}

UACAT_JumpAndWaitForLanding* UACAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UACAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UACAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UACAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UACAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UACAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UACAT_JumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UACAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}

