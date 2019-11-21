using UnrealBuildTool;
using System.IO;
public class SkiaUE4 : ModuleRules
{
  protected string ThirdPartyPath
  {
    get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "..", "ThirdParty")); }
  }
  public SkiaUE4(ReadOnlyTargetRules Target) : base(Target)
  {

    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

    bEnableUndefinedIdentifierWarnings = false;
    bTreatAsEngineModule = true;
    if (Load(Target))
    {
      PublicDefinitions.Add(string.Format("WITH_SKIA=1"));
    }
    else
    {
      PublicDefinitions.Add(string.Format("WITH_SKIA=0"));
    }
  }

  private bool Load(ReadOnlyTargetRules Target)
  {
    PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "skia", "include"));
    string LibrariesPath = Path.Combine(ThirdPartyPath, "skia", "lib");

    if (Target.Platform == UnrealTargetPlatform.Win64)
    {
      LibrariesPath = Path.Combine(LibrariesPath, "win");
      LibrariesPath = Path.Combine(LibrariesPath, Target.Configuration == UnrealTargetConfiguration.Debug ? "debug" : "release");

      PublicAdditionalLibraries.AddRange(
        new string[] {
          Path.Combine(LibrariesPath, "harfbuzz.lib"),
          Path.Combine(LibrariesPath, "skshaper.lib"),
          Path.Combine(LibrariesPath, "skottie.lib"),
          Path.Combine(LibrariesPath, "sksg.lib"),
          Path.Combine(LibrariesPath, "skia.lib"),
          Path.Combine(LibrariesPath, "icu.lib"),
        }
      );
      return true;
    }

    else if (Target.Platform == UnrealTargetPlatform.Mac)
    {
      return false;
    }

    return false;
  }
}
