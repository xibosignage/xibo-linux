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
    const std::string Options = "options";

    std::string attr(const std::string& property);
    std::string option(const std::string& property);

    const std::string LayoutNode = "layout";
    const std::string RegionNode = "region";
    const std::string MediaNode = "media";

    const std::string AudioType = "audio";
    const std::string VideoType = "video";
    const std::string ImageType = "image";

    struct LayoutOptions
    {
        LayoutOptions(const xlf_node& node);

        int width;
        int height;
        boost::optional<std::string> backgroundPath;
        boost::optional<std::string> backgroundColor;
    };

    struct RegionOptions
    {
        RegionOptions(const xlf_node& node);

        int width;
        int height;
        int left;
        int top;
        boost::optional<int> zindex;
        boost::optional<bool> loop;
    };

    struct MediaOptions
    {
        MediaOptions(const xlf_node& node);
        static std::string getType(const xlf_node& node);

        int id;
        boost::optional<std::string> uri;
        int duration;
    };

    struct ImageOptions : MediaOptions
    {
        ImageOptions(const xlf_node& node);

        boost::optional<std::string> scaleType;
        boost::optional<std::string> align;
        boost::optional<std::string> valign;
    };

    struct VideoOptions : MediaOptions
    {
        VideoOptions(const xlf_node& node);

        boost::optional<bool> muted;
        boost::optional<bool> looped;
    };

    struct AudioOptions : MediaOptions
    {
        AudioOptions(const xlf_node& node);

        boost::optional<bool> muted;
        boost::optional<bool> looped;
        boost::optional<int> volume;
    };

    struct WebViewOptions : MediaOptions
    {
        WebViewOptions(const xlf_node& node);

        boost::optional<bool> transparent;
    };
}

