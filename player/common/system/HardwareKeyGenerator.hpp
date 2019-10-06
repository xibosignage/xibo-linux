#pragma once

#include "common/system/HardwareKey.hpp"

#include <boost/process/io.hpp>
#include <string>

class HardwareKeyGenerator
{
public:
    static HardwareKey generate();

private:
    static std::string cpuid();
    static void nativeCpuid(unsigned int* eax, unsigned int* ebx, unsigned int* ecx, unsigned int* edx);
    static std::string macAddress();
    static std::string volumeSerial();
    static std::string retrieveVolumeSerial(boost::process::ipstream& stream);
};
