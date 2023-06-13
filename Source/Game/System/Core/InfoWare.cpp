#include "InfoWare.h"

UIMPL_SINGLETON(InfoWare)

InfoWare::InfoWare(const SpawnParams& params)
    : ISystem(params)
{
}

const char* InfoWare::GetVendorName(iware::gpu::vendor_t vendor)
{
    switch (vendor)
    {
    case iware::gpu::vendor_t::intel:
    {
        return "Intel";
    }
    case iware::gpu::vendor_t::amd:
    {
        return "AMD";
    }
    case iware::gpu::vendor_t::nvidia:
    {
        return "NVidia";
    }
    case iware::gpu::vendor_t::microsoft:
    {
        return "Microsoft";
    }
    case iware::gpu::vendor_t::qualcomm:
    {
        return "Qualcomm";
    }
    case iware::gpu::vendor_t::apple:
    {
        return "Apple";
    }
    default:
    {
        return "Unknown";
    }
    }
}

void InfoWare::SendSystemInfo()
{
    const auto memory = iware::system::memory();
    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", "Telemetry", "System", "Memory", "Physical", Utilities::BytesToText(memory.physical_total).ToStringAnsi()));
    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", "Telemetry", "System", "Memory", "Virtual", Utilities::BytesToText(memory.virtual_total).ToStringAnsi()));

    const auto os = iware::system::OS_info();
    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", "Telemetry", "System", "OS", "Name", os.full_name.c_str()));
    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}.{5}.{6}.{7}", "Telemetry", "System", "OS", "Version", os.major, os.minor, os.patch, os.build_number));

    HashSet<StringAnsi> resolutions;
    HashSet<StringAnsi> dpis;
    HashSet<StringAnsi> refreshs;

    const auto displays = iware::system::displays();

    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", "Telemetry", "System", "Display", "Count", displays.size()));

    for (auto i = 0u; i < displays.size(); ++i)
    {
        const auto& display = displays[i];
        resolutions.Add(StringAnsi::Format("{0}x{1}", display.width, display.height));
        dpis.Add(StringAnsi::Format("{0}", display.dpi));
        refreshs.Add(Utilities::HertzToText(static_cast<uint64_t>(display.refresh_rate)).ToStringAnsi());
    }

    for (const auto& resolution : resolutions)
    {
        Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", "Telemetry", "System", "Display", "Resolution", resolution.Item));
    }

    for (const auto& dpi : dpis)
    {
        Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", "Telemetry", "System", "Display", "DPI", dpi.Item));
    }

    for (const auto& refresh : refreshs)
    {
        Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}:{4}", "Telemetry", "System", "Display", "Refresh", refresh.Item));
    }
}

void InfoWare::SendCPUInfo()
{
    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}", "Telemetry", "CPU", "ModelName", iware::cpu::model_name().c_str()));
    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}", "Telemetry", "CPU", "VendorID", iware::cpu::vendor_id().c_str()));
}

void InfoWare::SendGPUInfo()
{
    HashSet<StringAnsi> vendors;
    HashSet<StringAnsi> names;

    const auto gpus = iware::gpu::device_properties();

    Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}", "Telemetry", "GPU", "Count", gpus.size()));

    for (auto i = 0u; i < gpus.size(); ++i)
    {
        const auto& gpu = gpus[i];
        vendors.Add(GetVendorName(gpu.vendor));
        names.Add(gpu.name.c_str());
    }

    for (const auto& vendor : vendors)
    {
        Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}", "Telemetry", "GPU", "Vendor", vendor.Item));
    }

    for (const auto& name : names)
    {
        Analytics::Instance->AddDesignEvent(StringAnsi::Format("{0}:{1}:{2}:{3}", "Telemetry", "GPU", "Name", name.Item));
    }
}

void InfoWare::OnInitialize()
{
    SendSystemInfo();
    SendCPUInfo();
    SendGPUInfo();
}
