#include "LaunchArgs.h"

LaunchArgs::LaunchArgs(const SpawnParams& params)
    : GameSystem(params)
{

}

String LaunchArgs::GetNextArgument(int currentIndex, const Array<String>& args)
{
    String Result;
    int NextIndex = currentIndex + 1;
    if (NextIndex < 0 || NextIndex > args.Count() - 1)
    {
        return Result;
    }
    return args[NextIndex];
}

void LaunchArgs::OnInitialize()
{
    String LaunchPath = TEXT("Default.launch");
    String CommandLine = Engine::GetCommandLine();

    if (CommandLine.Contains(TEXT(".launch"), StringSearchCase::CaseSensitive))
    {
        Array<String> SplitResult;
        CommandLine.Split(' ', SplitResult);
        for (int i = 0; i < SplitResult.Count(); ++i)
        {
            String Target = SplitResult[i];
            if (Target == TEXT("-l") || Target == TEXT("--launch"))
            {
                LaunchPath = GetNextArgument(i, SplitResult);
                break;
            }
        }
    }

    if (!FileSystem::FileExists(LaunchPath))
    {
        String ErrorText = TEXT("Could not find ") + LaunchPath + TEXT(" in order to launch the game");
#if BUILD_DEBUG
        DebugLog::LogError(ErrorText);
#endif
        Platform::Error(ErrorText);
        Engine::RequestExit(1);
        return;
    }

    Array<byte> Bytes;
    File::ReadAllBytes(LaunchPath, Bytes);

    _args = New<Args>();
    JsonSerializer::LoadFromBytes(_args, Bytes, Globals::EngineBuildNumber);
}

void LaunchArgs::OnDeinitialize()
{
    Delete(_args);
}

const Args* LaunchArgs::GetArgs()
{
    return _args;
}