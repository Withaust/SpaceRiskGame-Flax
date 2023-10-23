#include "LaunchArgs.h"

LaunchArgs* LaunchArgs::Instance = nullptr;

bool LaunchArgs::IsSteam = false;
String LaunchArgs::Name = TEXT("");
bool LaunchArgs::IsHost = true;
String LaunchArgs::Hostname = TEXT("");
bool LaunchArgs::IsModded = false;
String LaunchArgs::Build = TEXT("SpaceRisk");
String LaunchArgs::Character = TEXT("Default");
String LaunchArgs::Universe = TEXT("Default");
bool LaunchArgs::Autosave = false;
String LaunchArgs::Level = TEXT("Main");

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
    RegisterArguments();
    
    if (Name == TEXT(""))
    {
        IsSteam = true;
    }

    if (Hostname != TEXT(""))
    {
        IsHost = false;
    }

    if (Build != TEXT("SpaceRisk"))
    {
        IsModded = true;
    }
}

void LaunchArgs::RegisterArguments()
{
#define REGISTER_STRING(switchName, argName) if (target == TEXT(switchName)) argName = GetNextArgument(i, splitResult)
#define REGISTER_NUMBERIC(switchName, argName) if (target == TEXT(switchName)) StringUtils::Parse(GetNextArgument(i, splitResult).Get(), &argName)
#define REGISTER_BOOL(switchName, argName) if (target == TEXT(switchName)) argName = true;

    auto commandLine = Engine::GetCommandLine();
    Array<String> splitResult;
    commandLine.Split(' ', splitResult);

    for (int i = 0; i < splitResult.Count(); ++i)
    {
        String target = splitResult[i];
        REGISTER_STRING("-name", Name);
        REGISTER_STRING("-hostname", Hostname);
        REGISTER_STRING("-build", Build);
        REGISTER_STRING("-character", Character);
        REGISTER_STRING("-universe", Universe);
        REGISTER_BOOL("-autosave", Autosave);
    }

#undef REGISTER_STRING
#undef REGISTER_NUMBERIC
#undef REGISTER_BOOL
}
