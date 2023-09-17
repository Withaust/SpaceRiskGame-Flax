#include "Saver.h"

UIMPL_SINGLETON(Saver);

const Char* Saver::_characterPath = TEXT("PersistentData/Character/{0}.json");
const Char* Saver::_worldPath = TEXT("PersistentData/World/{0}.json");

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

void Saver::LoadWorld()
{
    BytesContainer data;
    File::ReadAllBytes(String::Format(_worldPath, _args->World.GetText()), data);
    SerializeDocument Document;
    Document.Parse(data.Get<char>(), data.Length());
    if (!Document.HasParseError())
    {
        World->Deserialize(Document, nullptr);
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

void Saver::SaveDefaultWorld()
{
    ScriptingObjectReference<WorldSave> world = New<WorldSave>();
    rapidjson_flax::StringBuffer buffer;
    PrettyJsonWriter writer(buffer);
    writer.StartObject();
    world->Serialize(writer, nullptr);
    writer.EndObject();
    File::WriteAllBytes(String::Format(_worldPath, TEXT("Default")), (byte*)buffer.GetString(), (int32)buffer.GetSize());
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

void Saver::SaveWorldAs(String Name)
{
    rapidjson_flax::StringBuffer buffer;
    PrettyJsonWriter writer(buffer);
    writer.StartObject();
    World->Serialize(writer, nullptr);
    writer.EndObject();
    File::WriteAllBytes(String::Format(_worldPath, Name.GetText()), (byte*)buffer.GetString(), (int32)buffer.GetSize());
}

void Saver::SaveCharacter()
{
    SaveCharacterAs(_args->Character);
}

void Saver::SaveWorld()
{
    SaveWorldAs(_args->World);
}

void Saver::OnInitialize()
{
    _args = LaunchArgs::Instance->GetArgs();

    LoadCharacter();
    if (_args->IsHost)
    {
        LoadWorld();
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
        SaveWorld();
    }

    UDEINIT_SINGLETON();
}