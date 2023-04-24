using Flax.Build;
using Flax.Build.NativeCpp;

public class GameEditorTarget : GameProjectEditorTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for editor
        Modules.Add("Game");
    }
}
