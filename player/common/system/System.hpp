#pragma once

#include "common/system/HardwareKey.hpp"
#include "common/system/Hostname.hpp"
#include "common/system/MacAddress.hpp"

namespace System
{
    MacAddress macAddress();
    HardwareKey hardwareKey();
    Hostname hostname();
    void preventSleep();
    void terminateProccess(int processId);
    int parentProcessId();
}
