#include "RegionParser.hpp"
#include "RegionResources.hpp"

#include "control/media/creators/MediaParser.hpp"
#include "control/media/creators/MediaParsersRepo.hpp"
#include "control/media/creators/MediaResources.hpp"

const int DefaultRegionZindex = 0;
const bool DefaultRegionLoop = false;

ParsedRegion RegionParser::parse(const xml_node& node)
{
    ParsedRegion region;

    region.options.id = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Region::Id));
    region.options.width = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Width)));
    region.options.height = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Height)));
    region.options.left = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Left)));
    region.options.top = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Top)));
    region.options.zindex = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Region::Zindex), DefaultRegionZindex);
    region.options.loop = static_cast<RegionOptions::Loop>(node.get<bool>(ResourcesXlf::option(ResourcesXlf::Region::Loop), DefaultRegionLoop));
    region.media = parseMedia(region.options.width, region.options.height, node);

    return region;
}

std::vector<ParsedMedia> RegionParser::parseMedia(int regionWidth, int regionHeight, const xml_node& node)
{
    std::vector<ParsedMedia> media;

    for(auto [nodeName, mediaNode] : node)
    {
        if(nodeName != ResourcesXlf::MediaNode) continue;

        auto&& parser = MediaParsersRepo::get(parseMediaType(mediaNode));
        if(parser)
        {
            auto parsedMedia = parser->parse(mediaNode);

            parsedMedia.extraOptions.emplace(ResourcesXlf::Region::Width, std::to_string(regionWidth));
            parsedMedia.extraOptions.emplace(ResourcesXlf::Region::Height, std::to_string(regionHeight));

            media.emplace_back(std::move(parsedMedia));
        }
    }

    return media;
}

MediaOptions::Type RegionParser::parseMediaType(const xml_node& node)
{
    auto type = node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type));
    auto render = node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Render));

    return {type, render};
}
