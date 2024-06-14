// P公司遭遇战,ZTC

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpTarget : TargetRules
{
    public ShootThemUpTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "ShootThemUp" });
    }
}
