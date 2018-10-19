#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

#include "constants.hpp"

class Resources
{
    static FilePath directory_;

public:
    static FilePath directory();
    static void setDirectory(const FilePath& directory);

};

namespace ResourcesXlf
{
    const std::string Attrs = "<xmlattr>";
    const std::string Options = "<options>";

    std::string attr(const std::string& property);
    std::string option(const std::string& property);

    namespace Layout
    {
        const std::string NodeName = "layout";
        const std::string Width = "width";
        const std::string Height = "height";
        const std::string BackgroundPath = "background";
        const std::string BackgroundColor = "bgcolor";

        int width(const xlf_node& node);
        int height(const xlf_node& node);
        boost::optional<std::string> backgroundPath(const xlf_node& node);
        boost::optional<std::string> backgroundColor(const xlf_node& node);
    }

    namespace Region
    {
        const std::string NodeName = "region";
        const std::string Width = "width";
        const std::string Height = "height";
        const std::string Left = "left";
        const std::string Top = "top";
        const std::string Zindex = "zindex";
        const std::string Loop = "loop";

        int width(const xlf_node& node);
        int height(const xlf_node& node);
        int left(const xlf_node& node);
        int top(const xlf_node& node);
        boost::optional<int> zindex(const xlf_node& node);
        boost::optional<bool> loop(const xlf_node& node);
    }

    namespace Media
    {
        const std::string NodeName = "media";
        const std::string Type = "type";

        const std::string AudioType = "audio";
        const std::string VideoType = "video";
        const std::string ImageType = "image";

        std::string type(const xlf_node& node);
    }
}

