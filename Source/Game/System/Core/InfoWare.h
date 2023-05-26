#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Utilities.h>
#include <Engine/Core/Collections/HashSet.h>

#include <Game/Shared/Defines.h>
#include <Game/System/Core/Analytics.h>

#include <InfoWare/system.hpp>
#include <InfoWare/cpu.hpp>
#include <InfoWare/gpu.hpp>

API_CLASS() class GAME_API InfoWare : public ISystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(InfoWare);

private:

    const char* GetVendorName(iware::gpu::vendor_t vendor);

    void SendSystemInfo();
    void SendCPUInfo();
    void SendGPUInfo();

public:
    void OnInitialize() override;
};
