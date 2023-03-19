#include "InfoWare.h"


InfoWare::InfoWare(const SpawnParams& params)
    : GameSystem(params)
{

}

void InfoWare::SendSystemInfo()
{
    const auto Memory = iware::system::memory();
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Memory:Physical:{0}", HumanReadable::ConvertBytes(Memory.physical_total).ToStringAnsi()));
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:Memory:Virtual:{0}", HumanReadable::ConvertBytes(Memory.virtual_total).ToStringAnsi()));

    const auto OS = iware::system::OS_info();
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:System:OS:Name:{0}", OS.full_name.c_str()));
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
        CPURefreshs.Add(HumanReadable::ConvertHertz(static_cast<uint64_t>(Display.refresh_rate)).ToStringAnsi());
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
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:CPU:ModelName:{0}", iware::cpu::model_name().c_str()));
    analytics->AddDesignEvent(StringAnsi::Format("Telemetry:CPU:VendorID:{0}", iware::cpu::vendor_id().c_str()));
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
        GPUNames.Add(GPU.name.c_str());
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
    analytics = GET_SYSTEM(GameAnalytics);

    SendSystemInfo();
    SendCPUInfo();
    SendGPUInfo();
}

void InfoWare::OnDeinitialize()
{

}
