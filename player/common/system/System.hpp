#pragma once

#include "common/system/HardwareKey.hpp"
#include "common/system/MacAddress.hpp"

namespace System
{
    struct DiskInfo
    {
        std::uintmax_t total;
        std::uintmax_t available;
    };

    MacAddress macAddress();
    HardwareKey hardwareKey();
    void preventSleep();
    void terminateProccess(int processId);
    int parentProcessId();
    DiskInfo diskInfo();
}
