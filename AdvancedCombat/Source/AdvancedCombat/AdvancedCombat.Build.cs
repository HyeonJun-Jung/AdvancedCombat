// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdvancedCombat : ModuleRules
{
	public AdvancedCombat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule"});

        PublicDependencyModuleNames.AddRange(new string[] {
            "GameFeatures",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });

        PrivateIncludePaths.Add("AdvancedCombat");
    }
}
