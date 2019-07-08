#pragma once

#include <string>

namespace ResourcesXlf
{
    const std::string Attrs = "<xmlattr>";
    const std::string Options = "options";

    std::string attr(const std::string& property);
    std::string option(const std::string& property);

    const std::string LayoutNode = "layout";
    const std::string RegionNode = "region";
    const std::string MediaNode = "media";
}
