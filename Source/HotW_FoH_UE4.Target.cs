// © 2026 Ricardo Sánchez Villegas. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HotW_FoH_UE4Target : TargetRules
{
	public HotW_FoH_UE4Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "HotW_FoH_UE4" } );
	}
}
