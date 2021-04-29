#include "HardwareKeyGenerator.hpp"

#include "common/Utils.hpp"
#include "common/system/System.hpp"
#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <regex>

namespace bp = boost::process;

HardwareKey HardwareKeyGenerator::generate()
{
    auto key = cpuid() + static_cast<std::string>(System::macAddress());

    return HardwareKey{Md5Hash::fromString(key)};
}

std::string HardwareKeyGenerator::cpuid()
{
    const int CPUID_BUFFER = 25;

    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;
    nativeCpuid(&eax, &ebx, &ecx, &edx);

    char buffer[CPUID_BUFFER] = {0};
    std::sprintf(buffer, "%08x%08x%08x", ebx, edx, ecx);

    return buffer;
}

inline void HardwareKeyGenerator::nativeCpuid(unsigned int* eax,
                                              unsigned int* ebx,
                                              unsigned int* ecx,
                                              unsigned int* edx)
{
    asm volatile("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "0"(*eax), "2"(*ecx) : "memory");
}

