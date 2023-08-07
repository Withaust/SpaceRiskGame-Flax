#include "LaunchArgs.h"

LaunchArgs* LaunchArgs::Instance = nullptr;

bool LaunchArgs::ForceClient = false;
String LaunchArgs::ForceWorld = String::Empty;
String LaunchArgs::ForceCharacter = String::Empty;

Args::Args(const SpawnParams& params)
    : ScriptingObject(params)
{
}

LaunchArgs::LaunchArgs(const SpawnParams& params)
    : ISystem(params)
{
}

String LaunchArgs::GetNextArgument(int currentIndex, const Array<String>& args)
{
    String result;
    int nextIndex = currentIndex + 1;
    if (nextIndex < 0 || nextIndex > args.Count() - 1)
    {
        return result;
    }
    return args[nextIndex];
}

void LaunchArgs::OnInitialize()
{
    String launchPath;
    if (ForceClient)
    {
        launchPath = TEXT("Client.launch");
        ForceClient = false;
    }
    else
    {
        launchPath = TEXT("Host.launch");
    }

    String commandLine = Engine::GetCommandLine();

    if (commandLine.Contains(TEXT(".launch"), StringSearchCase::CaseSensitive))
    {
        Array<String> splitResult;
        commandLine.Split(' ', splitResult);
        for (int i = 0; i < splitResult.Count(); ++i)
        {
            String target = splitResult[i];
            if (target == TEXT("-l") || target == TEXT("--launch"))
            {
                launchPath = GetNextArgument(i, splitResult);
                break;
            }
        }
    }

    if (!FileSystem::FileExists(launchPath))
    {
        String errorText = TEXT("Could not find ") + launchPath + TEXT(" in order to launch the game");
#if BUILD_DEBUG
        DebugLog::LogError(errorText);
#endif
        Platform::Error(errorText);
        Engine::RequestExit(1);
        return;
    }

    Array<byte> bytes;
    File::ReadAllBytes(launchPath, bytes);

    _args = New<Args>();
    JsonSerializer::LoadFromBytes(_args, bytes, Globals::EngineBuildNumber);

    if (ForceWorld != String::Empty)
    {
        _args->World = ForceWorld;
        ForceWorld = String::Empty;
    }

    if (ForceCharacter != String::Empty)
    {
        _args->Character = ForceCharacter;
        ForceCharacter = String::Empty;
    }
}

const ScriptingObjectReference<Args> LaunchArgs::GetArgs()
{
    return _args;
}
