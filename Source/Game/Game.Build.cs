using Flax.Build;
using Flax.Build.NativeCpp;

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

        Tags["Network"] = string.Empty;
        options.PublicDependencies.Add("Networking");
        if(options.Configuration != TargetConfiguration.Release)
        {
            options.ScriptingAPI.FileReferences.Add(System.IO.Path.Combine(FolderPath, "..", "..", "..", "ClosedXML", "System.IO.Packaging.dll"));
            options.ScriptingAPI.FileReferences.Add(System.IO.Path.Combine(FolderPath, "..", "..", "..", "ClosedXML", "DocumentFormat.OpenXml.dll"));
            options.ScriptingAPI.FileReferences.Add(System.IO.Path.Combine(FolderPath, "..", "..", "..", "ClosedXML", "SixLabors.Fonts.dll"));
            options.ScriptingAPI.FileReferences.Add(System.IO.Path.Combine(FolderPath, "..", "..", "..", "ClosedXML", "ExcelNumberFormat.dll"));
            options.ScriptingAPI.FileReferences.Add(System.IO.Path.Combine(FolderPath, "..", "..", "..", "ClosedXML", "ClosedXML.dll"));
            options.ScriptingAPI.SystemReferences.Add("System.Data.Common");
        }

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
        if (options.Configuration != TargetConfiguration.Debug)
        {
            options.PublicDependencies.Add("GameAnalytics");
        }
        options.PublicDependencies.Add("InfoWare");
		options.PublicDependencies.Add("RmlUi");
    }
}
