#include "HardwareKeyGenerator.hpp"

#include "common/Utils.hpp"
#include "common/system/System.hpp"

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <regex>

HardwareKey HardwareKeyGenerator::generate()
{
    auto key = cpuid() + static_cast<std::string>(System::macAddress()) + volumeSerial();

    return HardwareKey{key};
}

std::string HardwareKeyGenerator::cpuid()
{
    const int CPUID_BUFFER = 25;

    unsigned int eax = 0, ebx, ecx, edx;
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

std::string HardwareKeyGenerator::volumeSerial()
{
    namespace bp = boost::process;

    bp::pipe volumeInfo;
    bp::ipstream stream;

    bp::child udevadm("udevadm info /dev/sda", bp::std_out > volumeInfo);
    bp::child grep("grep ID_SERIAL_SHORT", bp::std_in<volumeInfo, bp::std_out> stream);

    auto volumeSerial = retrieveVolumeSerial(stream);

    udevadm.wait();
    grep.wait();

    return volumeSerial;
}

std::string HardwareKeyGenerator::retrieveVolumeSerial(boost::process::ipstream& stream)
{
    const int SERIAL_CAPTURE_GROUP = 1;
    const std::regex SERIAL_REGEX{"ID_SERIAL_SHORT=(.+)"};

    std::string line;
    std::getline(stream, line);

    std::smatch result;
    if (std::regex_search(line, result, SERIAL_REGEX))
    {
        return result[SERIAL_CAPTURE_GROUP].str();
    }

    return {};
}
