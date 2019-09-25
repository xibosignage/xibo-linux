#pragma once

#include <string>

namespace XlfResources
{
inline std::string option(const std::string& property)
{
    return "options." + property;
}

const std::string LayoutNode = "layout";
const std::string RegionNode = "region";
const std::string MediaNode = "media";
}
