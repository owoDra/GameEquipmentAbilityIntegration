// Copyright (C) 2024 owoDra

using UnrealBuildTool;

public class GIEquipAbility : ModuleRules
{
	public GIEquipAbility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[]
            {
                ModuleDirectory,
                ModuleDirectory + "/GIEquipAbility",
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "CoreUObject", "Engine",

                "GameplayAbilities",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "GEEquip", "GAExt",
            }
        );

    }
}
