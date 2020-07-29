#include "RegionParser.hpp"

#include "control/region/RegionImpl.hpp"
#include "control/region/RegionResources.hpp"

#include "control/media/MediaParsersRepo.hpp"

const int DefaultRegionZorder = 0;
const bool DefaultRegionLoop = false;

using namespace std::string_literals;

RegionParser::RegionParser(bool globalStatEnabled) : globalStatEnabled_{globalStatEnabled} {}

std::unique_ptr<Xibo::Region> RegionParser::regionFrom(const XmlNode& node)
{
    try
    {
        auto region = std::make_unique<RegionImpl>(optionsFrom(node));

        addMedia(*region, node);

        return region;
    }
    catch (PlayerRuntimeError& e)
    {
        throw Error{"RegionParser - " + e.domain(), e.message()};
    }
    catch (std::exception& e)
    {
        throw Error{"RegionParser", e.what()};
    }
}

RegionPosition RegionParser::positionFrom(const XmlNode& node)
{
    RegionPosition position;

    position.left = static_cast<int>(node.get<float>(XlfResources::Region::Left));
    position.top = static_cast<int>(node.get<float>(XlfResources::Region::Top));
    position.zorder = node.get<int>(XlfResources::Region::Zindex, DefaultRegionZorder);
    if (position.zorder < 0)
    {
        position.zorder = DefaultRegionZorder;
    }

    return position;
}

RegionOptions RegionParser::optionsFrom(const XmlNode& node)
{
    RegionOptions options;

    options.id = node.get<int>(XlfResources::Region::Id);
    options.width = static_cast<int>(node.get<float>(XlfResources::Region::Width));
    options.height = static_cast<int>(node.get<float>(XlfResources::Region::Height));
    options.loop = static_cast<RegionOptions::Loop>(node.get<bool>(XlfResources::Region::Loop, DefaultRegionLoop));

    return options;
}

void RegionParser::addMedia(Xibo::Region& region, const XmlNode& regionNode)
{
    for (auto [nodeName, node] : regionNode)
    {
        if (nodeName != XlfResources::MediaNode) continue;

        auto parser = MediaParsersRepo::get(mediaTypeFrom(node));
        if (parser)
        {
            // TODO: don't use width/height if media type is widget-less
            int width = region.view()->width();
            int height = region.view()->height();

            region.addMedia(parser->mediaFrom(node, width, height, globalStatEnabled_));
        }
    }
}

MediaOptions::Type RegionParser::mediaTypeFrom(const XmlNode& node)
{
    auto type = node.get<std::string>(XlfResources::Media::Type);
    auto render = node.get<std::string>(XlfResources::Media::Render);

    return {type, render};
}
