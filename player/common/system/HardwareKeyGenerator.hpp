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
};
