#pragma once

#include <Engine/Scripting/Script.h>

#include <Game/System/GameSystem.h>
#include <Game/System/GameAnalytics.h>
#include <Game/Util/HumanReadable.h>

#include <InfoWare/system.hpp>
#include <InfoWare/cpu.hpp>
#include <InfoWare/gpu.hpp>

API_CLASS() class GAME_API InfoWare : public GameSystem
{
    API_AUTO_SERIALIZATION();
    DECLARE_SCRIPTING_TYPE(InfoWare);

private:

    const char* GetKernelName(iware::system::kernel_t variant)
    {
        switch (variant)
        {
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

    const char* GetCacheTypeName(iware::cpu::cache_type_t cache_type)
    {
        switch (cache_type)
        {
        case iware::cpu::cache_type_t::unified:
            return "Unified";
        case iware::cpu::cache_type_t::instruction:
            return "Instruction";
        case iware::cpu::cache_type_t::data:
            return "Data";
        case iware::cpu::cache_type_t::trace:
            return "Trace";
        default:
            return "Unknown";
        }
    }

    const char* GetArchitectureName(iware::cpu::architecture_t architecture)
    {
        switch (architecture)
        {
        case iware::cpu::architecture_t::x64:
            return "x64";
        case iware::cpu::architecture_t::arm:
            return "ARM";
        case iware::cpu::architecture_t::itanium:
            return "Itanium";
        case iware::cpu::architecture_t::x86:
            return "x86";
        default:
            return "Unknown";
        }
    }

    const char* GetEndiannessName(iware::cpu::endianness_t endianness)
    {
        switch (endianness)
        {
        case iware::cpu::endianness_t::little:
            return "Little-Endian";
        case iware::cpu::endianness_t::big:
            return "Big-Endian";
        default:
            return "Unknown";
        }
    }

    const char* GetVendorName(iware::gpu::vendor_t vendor)
    {
        switch (vendor)
        {
        case iware::gpu::vendor_t::intel:
            return "Intel";
        case iware::gpu::vendor_t::amd:
            return "AMD";
        case iware::gpu::vendor_t::nvidia:
            return "NVidia";
        case iware::gpu::vendor_t::microsoft:
            return "Microsoft";
        case iware::gpu::vendor_t::qualcomm:
            return "Qualcomm";
        case iware::gpu::vendor_t::apple:
            return "Apple";
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
