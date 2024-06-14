// P公司遭遇战,ZTC

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpEditorTarget : TargetRules
{
    public ShootThemUpEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "ShootThemUp" });
    }
}
