#include "InfoWare.h"


InfoWare::InfoWare(const SpawnParams& params)
    : ISystem(params)
{

}

const char* InfoWare::GetVendorName(iware::gpu::vendor_t vendor)
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

void InfoWare::SendSystemInfo()
{
    const auto Memory = iware::system::memory();
    analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "Memory", "Physical", HumanReadable::ConvertBytesAnsi(Memory.physical_total)));
    analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "Memory", "Virtual", HumanReadable::ConvertBytesAnsi(Memory.virtual_total)));

    const auto OS = iware::system::OS_info();
    analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "OS", "Name", OS.full_name.c_str()));
    analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "OS", "Version", StringAnsi::Format("{0}.{1}.{2}.{3}", OS.major, OS.minor, OS.patch, OS.build_number)));

    HashSet<StringAnsi> CPUResolutions;
    HashSet<StringAnsi> CPUDPIs;
    HashSet<StringAnsi> CPURefreshs;

    const auto Displays = iware::system::displays();

    analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "Display", "Count", StringAnsi::Format("{0}", Displays.size())));

    for (auto i = 0u; i < Displays.size(); ++i)
    {
        const auto& Display = Displays[i];
        CPUResolutions.Add(StringAnsi::Format("{0}x{1}", Display.width, Display.height));
        CPUDPIs.Add(StringAnsi::Format("{0}", Display.dpi));
        CPURefreshs.Add(HumanReadable::ConvertHertzAnsi(static_cast<uint64_t>(Display.refresh_rate)));
    }

    for (const auto& Resolution : CPUResolutions)
    {
        analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "Display", "Resolution", Resolution.Item));
    }

    for (const auto& DPI : CPUDPIs)
    {
        analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "Display", "DPI", DPI.Item));
    }

    for (const auto& Refresh : CPURefreshs)
    {
        analytics->AddDesignEvent(DesignEvent("Telemetry", "System", "Display", "Refresh", Refresh.Item));
    }
}

void InfoWare::SendCPUInfo()
{
    analytics->AddDesignEvent(DesignEvent("Telemetry", "CPU", "ModelName", iware::cpu::model_name().c_str()));
    analytics->AddDesignEvent(DesignEvent("Telemetry", "CPU", "VendorID", iware::cpu::vendor_id().c_str()));
}

void InfoWare::SendGPUInfo()
{
    HashSet<StringAnsi> GPUVendors;
    HashSet<StringAnsi> GPUNames;

    const auto GPUs = iware::gpu::device_properties();

    analytics->AddDesignEvent(DesignEvent("Telemetry", "GPU", "Count", StringAnsi::Format("{0}", GPUs.size())));

    for (auto i = 0u; i < GPUs.size(); ++i)
    {
        const auto& GPU = GPUs[i];
        GPUVendors.Add(GetVendorName(GPU.vendor));
        GPUNames.Add(GPU.name.c_str());
    }

    for (const auto& Vendor : GPUVendors)
    {
        analytics->AddDesignEvent(DesignEvent("Telemetry", "GPU", "Vendor", Vendor.Item));
    }

    for (const auto& Name : GPUNames)
    {
        analytics->AddDesignEvent(DesignEvent("Telemetry", "GPU", "Name", Name.Item));
    }
}

void InfoWare::OnInitialize()
{
    analytics = Analytics::Get();

    SendSystemInfo();
    SendCPUInfo();
    SendGPUInfo();
}

void InfoWare::OnDeinitialize()
{

}
