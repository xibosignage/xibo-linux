#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

#include "constants.hpp"

namespace ResourcesXlf
{
    const std::string Attrs = "<xmlattr>";
    const std::string Options = "<options>";

    std::string attr(const std::string& property)
    {
        return Attrs + "." + property;
    }

    std::string option(const std::string& property)
    {
        return Options + "." + property;
    }

    namespace Layout
    {
        const std::string NodeName = "layout";
        const std::string Width = "width";
        const std::string Height = "height";
        const std::string BackgroundPath = "background";
        const std::string BackgroundColor = "bgcolor";

        int width(const xlf_node& node)
        {
            return node.get<int>(attr(Width));
        }

        int height(const xlf_node& node)
        {
            return node.get<int>(attr(Height));
        }

        boost::optional<std::string> backgroundPath(const xlf_node& node)
        {
            return node.get_optional<std::string>(attr(BackgroundPath));
        }

        boost::optional<std::string> backgroundColor(const xlf_node& node)
        {
            return node.get_optional<std::string>(attr(BackgroundColor));
        }
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

        int width(const xlf_node& node)
        {
            return static_cast<int>(node.get<float>(attr(Width)));
        }

        int height(const xlf_node& node)
        {
            return static_cast<int>(node.get<float>(attr(Height)));
        }

        int left(const xlf_node& node)
        {
            return static_cast<int>(node.get<float>(attr(Left)));
        }

        int top(const xlf_node& node)
        {
            return static_cast<int>(node.get<float>(attr(Top)));
        }

        boost::optional<int> zindex(const xlf_node& node)
        {
            return node.get_optional<int>(attr(Zindex));
        }

        boost::optional<bool> loop(const xlf_node& node)
        {
            return node.get_optional<bool>(option(Loop));
        }
    }

    namespace Media
    {
        const std::string NodeName = "media";
        const std::string Type = "type";

        const std::string AudioType = "audio";
        const std::string VideoType = "video";
        const std::string ImageType = "image";

        std::string type(const xlf_node& node)
        {
            return node.get<std::string>(attr(Type));
        }
    }
}
