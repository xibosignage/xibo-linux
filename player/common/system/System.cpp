#include "System.hpp"

#include "common/system/Dbus.hpp"
#include "common/system/HardwareKeyGenerator.hpp"
#include "common/system/MacAddressFetcher.hpp"

#include <signal.h>

MacAddress System::macAddress()
{
    static MacAddress address{MacAddressFetcher::fetch()};
    return address;
}

HardwareKey System::hardwareKey()
{
    static auto key{HardwareKeyGenerator::generate()};
    return key;
}

void System::preventSleep()
{
    Dbus dbus;
    dbus.preventSleep();
}

void System::terminateProccess(int processId)
{
    kill(processId, SIGTERM);
}

int System::parentProcessId()
{
    return getppid();
}
