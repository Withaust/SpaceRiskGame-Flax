#include "Saver.h"

UIMPL_SINGLETON(Saver);

const Char* Saver::_characterPath = TEXT("PersistentData/Character/{0}.json");
const Char* Saver::_universePath = TEXT("PersistentData/Universe/{0}.json");

Saver::Saver(const SpawnParams& params)
    : ISystem(params)
{
}

void Saver::LoadCharacter()
{
    BytesContainer data;
    File::ReadAllBytes(String::Format(_characterPath, _args->Character.GetText()), data);
    SerializeDocument Document;
    Document.Parse(data.Get<char>(), data.Length());
    if (!Document.HasParseError())
    {
        Character->Deserialize(Document, nullptr);
    }    
}

void Saver::LoadUniverse()
{
    BytesContainer data;
    File::ReadAllBytes(String::Format(_universePath, _args->Universe.GetText()), data);
    SerializeDocument Document;
    Document.Parse(data.Get<char>(), data.Length());
    if (!Document.HasParseError())
    {
        Universe->Deserialize(Document, nullptr);
    }
}

void Saver::SaveDefaultCharacter()
{
    ScriptingObjectReference<CharacterSave> character = New<CharacterSave>();
    rapidjson_flax::StringBuffer buffer;
    PrettyJsonWriter writer(buffer);
    writer.StartObject();
    character->Serialize(writer, nullptr);
    writer.EndObject();
    File::WriteAllBytes(String::Format(_characterPath, TEXT("Default")), (byte*)buffer.GetString(), (int32)buffer.GetSize());
}

void Saver::SaveDefaultUniverse()
{
    ScriptingObjectReference<UniverseSave> universe = New<UniverseSave>();
    rapidjson_flax::StringBuffer buffer;
    PrettyJsonWriter writer(buffer);
    writer.StartObject();
    universe->Serialize(writer, nullptr);
    writer.EndObject();
    File::WriteAllBytes(String::Format(_universePath, TEXT("Default")), (byte*)buffer.GetString(), (int32)buffer.GetSize());
}

void Saver::SaveCharacterAs(String Name)
{
    rapidjson_flax::StringBuffer buffer;
    PrettyJsonWriter writer(buffer);
    writer.StartObject();
    Character->Serialize(writer, nullptr);
    writer.EndObject();
    File::WriteAllBytes(String::Format(_characterPath, Name.GetText()), (byte*)buffer.GetString(), (int32)buffer.GetSize());
}

void Saver::SaveUniverseAs(String Name)
{
    rapidjson_flax::StringBuffer buffer;
    PrettyJsonWriter writer(buffer);
    writer.StartObject();
    Universe->Serialize(writer, nullptr);
    writer.EndObject();
    File::WriteAllBytes(String::Format(_universePath, Name.GetText()), (byte*)buffer.GetString(), (int32)buffer.GetSize());
}

void Saver::SaveCharacter()
{
    SaveCharacterAs(_args->Character);
}

void Saver::SaveUniverse()
{
    SaveUniverseAs(_args->Universe);
}

void Saver::OnInitialize()
{
    _args = LaunchArgs::Instance;

    LoadCharacter();
    if (_args->IsHost)
    {
        LoadUniverse();
    }
}

void Saver::OnDeinitialize()
{
    if (!_args->Autosave)
    {
        return;
    }

    SaveCharacter();
    if (_args->IsHost)
    {
        SaveUniverse();
    }

    UDEINIT_SINGLETON();
}