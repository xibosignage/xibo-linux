#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace Resources
{
    const std::string Attrs = "<xmlattr>";

    namespace Layout
    {
        const std::string NodeName = "layout";
    }

    namespace Region
    {
        const std::string NodeName = "region";
        const std::string Left = "left";
        const std::string Top = "top";
    }

    namespace Media
    {
        const std::string NodeName = "media";
        const std::string Type = "type";

        const std::string AudioType = "audio";
        const std::string VideoType = "video";
        const std::string ImageType = "image";
    }
}
