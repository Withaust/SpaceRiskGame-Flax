#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/System/GameSystem.h>
#include <Game/System/GameAnalytics.h>

#include <InfoWare/system.hpp>
#include <InfoWare/cpu.hpp>
#include <InfoWare/gpu.hpp>

API_CLASS() class GAME_API InfoWare : public GameSystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(InfoWare);

private:

    const char* GetKernelName(iware::system::kernel_t variant) noexcept {
        switch (variant) {
        case iware::system::kernel_t::windows_nt:
            return "Windows NT";
        case iware::system::kernel_t::linux:
            return "Linux";
        case iware::system::kernel_t::darwin:
            return "Darwin";
        default:
            return "Unknown";
        }
    }

    GameAnalytics* analytics = nullptr;

    void SendSystemInfo();
    void SendCPUInfo();
    void SendGPUInfo();

public:
    void OnInitialize() override;
    void OnDeinitialize() override;
};
