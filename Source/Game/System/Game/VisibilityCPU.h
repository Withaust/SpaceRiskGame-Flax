#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Physics/Physics.h>
#include <Engine/Core/Types/LayersMask.h>

#include <Game/Utils/Defines.h>

API_CLASS() class GAME_API VisibilityCPU : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(VisibilityCPU);
    USINGLETON(VisibilityCPU);

private:

public:

    API_FIELD() LayersMask RaycastMask;

    void OnInitialize() override;
    void OnDeinitialize() override;
    bool Queue(Vector3 Origin, Vector3 Direction);
};
