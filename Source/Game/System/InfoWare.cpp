#include "InfoWare.h"


InfoWare::InfoWare(const SpawnParams& params)
    : GameSystem(params)
{

}

void InfoWare::SendSystemInfo()
{
    // System
    const auto Memory = iware::system::memory();
    StringAnsi MemoryPhysicalAvailable = StringAnsi::Format("Telemetry:System:Memory:PhysicalAvailable:{0}", HumanReadable::ConvertBytes(Memory.physical_available).ToStringAnsi());
    analytics->AddDesignEvent(MemoryPhysicalAvailable.GetText());
    StringAnsi MemoryPhysicalTotal = StringAnsi::Format("Telemetry:System:Memory:PhysicalTotal:{0}", HumanReadable::ConvertBytes(Memory.physical_total).ToStringAnsi());
    analytics->AddDesignEvent(MemoryPhysicalTotal.GetText());
    StringAnsi MemoryVirtualAvailable = StringAnsi::Format("Telemetry:System:Memory:VirtualAvailable:{0}", HumanReadable::ConvertBytes(Memory.virtual_available).ToStringAnsi());
    analytics->AddDesignEvent(MemoryVirtualAvailable.GetText());
    StringAnsi MemoryVirtualTotal = StringAnsi::Format("Telemetry:System:Memory:VirtualTotal:{0}", HumanReadable::ConvertBytes(Memory.virtual_total).ToStringAnsi());
    analytics->AddDesignEvent(MemoryVirtualTotal.GetText());

    const auto OS = iware::system::OS_info();
    StringAnsi OSFullName = StringAnsi::Format("Telemetry:System:OS:FullName:{0}", OS.full_name.c_str());
    analytics->AddDesignEvent(OSFullName.GetText());
    StringAnsi OSVersion = StringAnsi::Format("Telemetry:System:OS:Version:{0}.{1}.{2}.{3}", OS.major, OS.minor, OS.patch, OS.build_number);
    analytics->AddDesignEvent(OSVersion.GetText());

    const auto Displays = iware::system::displays();
    for (auto i = 0u; i < Displays.size(); ++i)
    {
        const auto& Display = Displays[i];
        StringAnsi DisplayResolution = StringAnsi::Format("Telemetry:System:Display:Resolution:{0}x{1}", Display.width, Display.height);
        analytics->AddDesignEvent(DisplayResolution.GetText(), static_cast<double>(i));
        StringAnsi DisplayDPI = StringAnsi::Format("Telemetry:System:Display:DPI:{0}", Display.dpi);
        analytics->AddDesignEvent(DisplayDPI.GetText(), static_cast<double>(i));
        StringAnsi DisplayBPP = StringAnsi::Format("Telemetry:System:Display:BPP:{0}", Display.bpp);
        analytics->AddDesignEvent(DisplayBPP.GetText(), static_cast<double>(i));
        StringAnsi DisplayRefresh = StringAnsi::Format("Telemetry:System:Display:Refresh:{0}", HumanReadable::ConvertHertz(static_cast<uint64_t>(Display.refresh_rate)).ToStringAnsi());
        analytics->AddDesignEvent(DisplayRefresh.GetText(), static_cast<double>(i));
    }

    // CPU

    StringAnsi CPUArchitecture = StringAnsi::Format("Telemetry:CPU:Architecture:{0}", GetArchitectureName(iware::cpu::architecture()));
    analytics->AddDesignEvent(CPUArchitecture.GetText());
    StringAnsi CPUFrequency = StringAnsi::Format("Telemetry:CPU:Frequency:{0}", HumanReadable::ConvertHertz(iware::cpu::frequency()).ToStringAnsi());
    analytics->AddDesignEvent(CPUFrequency.GetText());
    StringAnsi CPUEndianness = StringAnsi::Format("Telemetry:CPU:Endianness:{0}", GetEndiannessName(iware::cpu::endianness()));
    analytics->AddDesignEvent(CPUEndianness.GetText());
    StringAnsi CPUModelName = StringAnsi::Format("Telemetry:CPU:ModelName:{0}", iware::cpu::model_name().c_str());
    analytics->AddDesignEvent(CPUModelName.GetText());
    StringAnsi CPUVendorID = StringAnsi::Format("Telemetry:CPU:VendorID:{0}", iware::cpu::vendor_id().c_str());
    analytics->AddDesignEvent(CPUVendorID.GetText());

    // GPU

    const auto GPUs = iware::gpu::device_properties();
    for (auto i = 0u; i < GPUs.size(); ++i) 
    {
        const auto& GPU = GPUs[i];
        StringAnsi GPUVendor = StringAnsi::Format("Telemetry:GPU:Vendor:{0}", GetVendorName(GPU.vendor));
        analytics->AddDesignEvent(GPUVendor.GetText(), static_cast<double>(i));
        StringAnsi GPUName = StringAnsi::Format("Telemetry:GPU:Name:{0}", GPU.name.c_str());
        analytics->AddDesignEvent(GPUName.GetText(), static_cast<double>(i));
        StringAnsi GPURAM = StringAnsi::Format("Telemetry:GPU:RAM:{0}", HumanReadable::ConvertBytes(GPU.memory_size).ToStringAnsi());
        analytics->AddDesignEvent(GPURAM.GetText(), static_cast<double>(i));
        StringAnsi GPUCache = StringAnsi::Format("Telemetry:GPU:Cache:{0}", HumanReadable::ConvertBytes(GPU.cache_size).ToStringAnsi());
        analytics->AddDesignEvent(GPUCache.GetText(), static_cast<double>(i));
        StringAnsi GPUFrequency = StringAnsi::Format("Telemetry:GPU:Frequency:{0}", HumanReadable::ConvertHertz(GPU.max_frequency).ToStringAnsi());
        analytics->AddDesignEvent(GPUFrequency.GetText(), static_cast<double>(i));
    }
}

void InfoWare::SendCPUInfo()
{

}

void InfoWare::SendGPUInfo()
{

}

void InfoWare::OnInitialize()
{
    analytics = GET_SYSTEM(GameAnalytics);

    SendSystemInfo();
}

void InfoWare::OnDeinitialize()
{

}
