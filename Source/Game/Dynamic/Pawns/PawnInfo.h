#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Networking/INetworkObject.h>

#include <Game/Shared/Defines.h>
#include <Game/Shared/Components/IComponent.h>
#include <Game/System/Core/Steam.h>

API_CLASS(Attributes = "ContentContextMenu(\"New/Data/Pawn/PawnData\")")
class GAME_API PawnData : public ISerializable
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PawnData);

    API_FIELD() String Name = TEXT("Pawn");
    API_FIELD() float Health = 100.0f;
};

API_CLASS() class GAME_API PawnInfo : public IComponent, public INetworkObject
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(PawnInfo);

private:

    String _customName = String::Empty;

public:

    API_PROPERTY(NetworkReplicated) String GetNameLocal() const;
    API_PROPERTY(NetworkReplicated) void SetNameLocal(String value);
    API_FUNCTION(NetworkRpc = "Server, Reliable") void SetNameRemote(const String& value) { UIMPL_NETPROP_SETREMOTE(PawnInfo, Name); }
    API_FUNCTION(NetworkRpc = "Client, Reliable") void SetNameSync(const String& value) { UIMPL_NETPROP_SETSYNC(PawnInfo, Name); }

    API_FIELD(NetworkReplicated, Attributes = "AssetReference(typeof(PawnData))") AssetReference<JsonAsset> Data;
    UDECLARE_DATA(PawnData, Data);
};
