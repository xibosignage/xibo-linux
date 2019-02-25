#include "XlfResources.hpp"

std::string ResourcesXlf::attr(const std::string& property)
{
    return Attrs + "." + property;
}

std::string ResourcesXlf::option(const std::string& property)
{
    return Options + "." + property;
}

