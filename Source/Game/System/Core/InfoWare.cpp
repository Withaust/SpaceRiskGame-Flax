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
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Memory:Physical:{0}", HumanReadable::ConvertBytesAnsi(Memory.physical_total)));
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Memory:Virtual:{0}", HumanReadable::ConvertBytesAnsi(Memory.virtual_total)));

    const auto OS = iware::system::OS_info();
    StringAnsi OSFullName = OS.full_name.c_str();
    analytics->FilterDesignAnsi(OSFullName);
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:OS:Name:{0}", OSFullName));
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:OS:Version:{0}.{1}.{2}.{3}", OS.major, OS.minor, OS.patch, OS.build_number));

    HashSet<StringAnsi> CPUResolutions;
    HashSet<StringAnsi> CPUDPIs;
    HashSet<StringAnsi> CPURefreshs;

    const auto Displays = iware::system::displays();

    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Display:Count:{0}", Displays.size()));

    for (auto i = 0u; i < Displays.size(); ++i)
    {
        const auto& Display = Displays[i];
        CPUResolutions.Add(StringAnsi::Format("{0}x{1}", Display.width, Display.height));
        CPUDPIs.Add(StringAnsi::Format("{0}", Display.dpi));
        CPURefreshs.Add(HumanReadable::ConvertHertzAnsi(static_cast<uint64_t>(Display.refresh_rate)));
    }

    for (const auto& Resolution : CPUResolutions)
    {
        analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Display:Resolution:{0}", Resolution.Item));
    }

    for (const auto& DPI : CPUDPIs)
    {
        analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Display:DPI:{0}", DPI.Item));
    }

    for (const auto& Refresh : CPURefreshs)
    {
        analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Display:Refresh:{0}", Refresh.Item));
    }
}

void InfoWare::SendCPUInfo()
{
    StringAnsi CPUName = iware::cpu::model_name().c_str();
    analytics->FilterDesignAnsi(CPUName);
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:CPU:ModelName:{0}", CPUName));
    StringAnsi CPUVendor = iware::cpu::vendor_id().c_str();
    analytics->FilterDesignAnsi(CPUVendor);
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:CPU:VendorID:{0}", CPUVendor));
}

void InfoWare::SendGPUInfo()
{
    HashSet<StringAnsi> GPUVendors;
    HashSet<StringAnsi> GPUNames;

    const auto GPUs = iware::gpu::device_properties();

    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:GPU:Count:{0}", GPUs.size()));

    for (auto i = 0u; i < GPUs.size(); ++i)
    {
        const auto& GPU = GPUs[i];
        GPUVendors.Add(GetVendorName(GPU.vendor));
        StringAnsi GPUName = GPU.name.c_str();
        analytics->FilterDesignAnsi(GPUName);
        GPUNames.Add(GPUName);
    }

    for (const auto& Vendor : GPUVendors)
    {
        analytics->AddDesignEvent(StringAnsi::Format("Telemetry:GPU:Vendor:{0}", Vendor.Item));
    }

    for (const auto& Name : GPUNames)
    {
        analytics->AddDesignEvent(StringAnsi::Format("Telemetry:GPU:Name:{0}", Name.Item));
    }
}

void InfoWare::OnInitialize()
{
    analytics = GET_SYSTEM(Analytics);

    SendSystemInfo();
    SendCPUInfo();
    SendGPUInfo();
}

void InfoWare::OnDeinitialize()
{

}
