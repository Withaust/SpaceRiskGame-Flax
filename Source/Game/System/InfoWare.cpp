#include "InfoWare.h"


InfoWare::InfoWare(const SpawnParams& params)
    : GameSystem(params)
{

}

void InfoWare::SendSystemInfo()
{
	analytics->AddDesignEvent("Telemetry:System:HID:Mice", static_cast<double>(iware::system::mouse_amount()));
	analytics->AddDesignEvent("Telemetry:System:HID:Keyboards", static_cast<double>(iware::system::keyboard_amount()));
	analytics->AddDesignEvent("Telemetry:System:HID:Other", static_cast<double>(iware::system::other_HID_amount()));

	const auto memory = iware::system::memory();
	analytics->AddDesignEvent("Telemetry:System:Memory:Physical:Available", static_cast<double>(memory.physical_available));
	analytics->AddDesignEvent("Telemetry:System:Memory:Physical:Total", static_cast<double>(memory.physical_total));
	analytics->AddDesignEvent("Telemetry:System:Memory:Virtual:Available", static_cast<double>(memory.virtual_available));
	analytics->AddDesignEvent("Telemetry:System:Memory:Virtual:Total", static_cast<double>(memory.virtual_total));

	const auto kernel_info = iware::system::kernel_info();
	StringAnsi KernelVariant = "Telemetry:System:Kernel:Variant:" + StringAnsi(GetKernelName(kernel_info.variant));
	analytics->AddDesignEvent(KernelVariant.GetText());
	StringAnsi KernelVersion = StringAnsi::Format("Telemetry:System:Kernel:Version:{0}.{1}.{2}.{3}", kernel_info.major, kernel_info.minor, kernel_info.patch, kernel_info.build_number);
	analytics->AddDesignEvent(KernelVersion.GetText());

	const auto OS_info = iware::system::OS_info();
	StringAnsi OSName = "Telemetry:System:OS:Name:" + StringAnsi(OS_info.name.c_str());
	analytics->AddDesignEvent(OSName.GetText());
	StringAnsi OSFullName = "Telemetry:System:OS:FullName:" + StringAnsi(OS_info.full_name.c_str());
	analytics->AddDesignEvent(OSFullName.GetText());
	StringAnsi OSVersion = StringAnsi::Format("Telemetry:System:OS:Version:{0}.{1}.{2}.{3}", OS_info.major, OS_info.minor, OS_info.patch, OS_info.build_number);
	analytics->AddDesignEvent(OSVersion.GetText());

	const auto displays = iware::system::displays();
	for (auto i = 0u; i < displays.size(); ++i)
	{
		const auto& display = displays[i];
		StringAnsi DisplayResolution = StringAnsi::Format("Telemetry:System:Display:Resolution:{0}x{1}", display.width, display.height);
		analytics->AddDesignEvent(DisplayResolution.GetText(), static_cast<double>(i));
		StringAnsi DisplayDPI = StringAnsi::Format("Telemetry:System:Display:DPI:{0}", display.dpi);
		analytics->AddDesignEvent(DisplayDPI.GetText(), static_cast<double>(i));
		StringAnsi DisplayBPP = StringAnsi::Format("Telemetry:System:Display:BPP:{0}", display.bpp);
		analytics->AddDesignEvent(DisplayBPP.GetText(), static_cast<double>(i));
		StringAnsi DisplayRefresh = StringAnsi::Format("Telemetry:System:Display:Refresh:{0}", display.refresh_rate);
		analytics->AddDesignEvent(DisplayRefresh.GetText(), static_cast<double>(i));
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
