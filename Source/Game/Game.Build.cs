using Flax.Build;
using Flax.Build.NativeCpp;
using System.IO;

public class Game : GameModule
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        BuildNativeCode = true;
    }

    /// <inheritdoc />
    public override void Setup(BuildOptions options)
    {
        base.Setup(options);

        options.ScriptingAPI.IgnoreMissingDocumentationWarnings = true;
		options.ScriptingAPI.SystemReferences.Add("System.Text.RegularExpressions");

        Tags["Network"] = string.Empty;
        options.PublicDependencies.Add("Networking");
        options.PublicDefinitions.Add("COMPILE_WITH_TEXTURE_TOOL");

        switch (options.Platform.Target)
        {
            case TargetPlatform.Windows:
                {
                    options.LinkEnv.CustomArgs.Add("/IGNORE:4099");
                    options.LinkEnv.CustomArgs.Add("/IGNORE:4098");
                    break;
                }
            case TargetPlatform.Linux:
            case TargetPlatform.Mac:
                {
                    // TODO: Add pdb linking ignores for other platforms as well
                    break;
                }
        }

        options.PublicDependencies.Add("SteamWorks");
        options.PublicDependencies.Add("GameAnalytics");
        options.PublicDependencies.Add("RmlUi");
    }
}
