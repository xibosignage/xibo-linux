#pragma once

#include "common/system/HardwareKey.hpp"

#include <ios>
#include <regex>
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
    static std::string currentDrive();
    static std::string executeAndGrepFirstLine(const std::string& command, const std::string& grepSearch);
    static std::string retrieveResult(const std::regex& regex, const std::string& line);
};
