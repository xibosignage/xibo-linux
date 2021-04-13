#include "HardwareKeyGenerator.hpp"

#include "common/Utils.hpp"
#include "common/system/System.hpp"
#include "config/AppConfig.hpp"

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <regex>

namespace bp = boost::process;

HardwareKey HardwareKeyGenerator::generate()
{
    auto key = cpuid() + static_cast<std::string>(System::macAddress()) + volumeSerial();

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

std::string HardwareKeyGenerator::volumeSerial()
{
    const std::string SERIAL_PREFIX{"ID_SERIAL_SHORT"};

    auto line = executeAndGrepFirstLine("udevadm info " + currentDrive(), SERIAL_PREFIX);

    return retrieveResult(std::regex{SERIAL_PREFIX + "=(.+)"}, line);
}

std::string HardwareKeyGenerator::currentDrive()
{
    const std::string DEVICE_PREFIX{"/dev/"};

    auto line = executeAndGrepFirstLine("df " + AppConfig::playerBinary(), DEVICE_PREFIX);

    return DEVICE_PREFIX + retrieveResult(std::regex{DEVICE_PREFIX + "([^\\s]+)"}, line);
}

std::string HardwareKeyGenerator::retrieveResult(const std::regex& regex, const std::string& line)
{
    const int SERIAL_CAPTURE_GROUP = 1;

    std::smatch result;
    if (std::regex_search(line, result, regex))
    {
        return result[SERIAL_CAPTURE_GROUP].str();
    }

    return {};
}

std::string HardwareKeyGenerator::executeAndGrepFirstLine(const std::string& command, const std::string& grepSearch)
{
    bp::pipe pipe;
    bp::ipstream stream;

    bp::child commandProcess(command, bp::std_out > pipe);
    bp::child grepProcess("grep " + grepSearch, bp::std_in<pipe, bp::std_out> stream);

    commandProcess.wait();
    grepProcess.wait();

    std::string line;
    std::getline(stream, line);
    return line;
}
