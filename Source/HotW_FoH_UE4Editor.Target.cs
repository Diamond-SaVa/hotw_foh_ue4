// © 2026 Ricardo Sánchez Villegas. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HotW_FoH_UE4EditorTarget : TargetRules
{
	public HotW_FoH_UE4EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "HotW_FoH_UE4" } );
	}
}
