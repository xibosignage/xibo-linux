#include "RegionOptions.hpp"
#include "common/Utils.hpp"

template <>
std::string Utils::toString(RegionOptions::Loop val)
{
    switch (val)
    {
        case RegionOptions::Loop::Disable: return "Disable";
        case RegionOptions::Loop::Enable: return "Enable";
    }

    return "unknown";
}
