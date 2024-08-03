// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvancedCombatGameMode.h"
#include "AdvancedCombatCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAdvancedCombatGameMode::AAdvancedCombatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
