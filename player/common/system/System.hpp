#pragma once

#include "common/system/HardwareKey.hpp"
#include "common/system/MacAddress.hpp"

namespace System
{
    MacAddress macAddress();
    HardwareKey hardwareKey();
    void preventSleep();
    void terminateProccess(int processId);
    int parentProcessId();
}
