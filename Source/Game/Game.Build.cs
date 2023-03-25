using Flax.Build;
using Flax.Build.NativeCpp;

public class Game : GameModule
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // C#-only scripting
        BuildNativeCode = true;
    }

    /// <inheritdoc />
    public override void Setup(BuildOptions options)
    {
        base.Setup(options);

        options.ScriptingAPI.IgnoreMissingDocumentationWarnings = true;
		
        Tags["Network"] = string.Empty;
        options.PublicDependencies.Add("Networking");
		
		switch (options.Platform.Target)
		{
		case TargetPlatform.Windows:
		case TargetPlatform.Linux:
		case TargetPlatform.Mac:
			options.PublicDependencies.Add("SteamWorks");
			options.PublicDependencies.Add("GameAnalytics");
			options.PublicDependencies.Add("InfoWare");
			break;
		}

        // Here you can modify the build options for your game module
        // To reference another module use: options.PublicDependencies.Add("Audio");
        // To add C++ define use: options.PublicDefinitions.Add("COMPILE_WITH_FLAX");
        // To learn more see scripting documentation.
    }
}
