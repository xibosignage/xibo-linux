#include "RegionParser.hpp"
#include "GetMediaPosition.hpp"
#include "Region.hpp"
#include "RegionResources.hpp"
#include "RegionView.hpp"

#include "control/media/MediaCreatorsRepo.hpp"

const int DefaultRegionZindex = 0;
const bool DefaultRegionLoop = false;

std::unique_ptr<IRegion> RegionParser::regionFrom(const ptree_node& node)
{
    auto options = optionsFrom(node);
    auto view = createView(options);
    auto region = createRegion(options, view);

    addMedia(*region, node);

    return region;
}

RegionOptions RegionParser::optionsFrom(const ptree_node& node)
{
    RegionOptions options;

    options.id = node.get<int>(XlfResources::Region::Id);
    options.width = static_cast<int>(node.get<float>(XlfResources::Region::Width));
    options.height = static_cast<int>(node.get<float>(XlfResources::Region::Height));
    options.left = static_cast<int>(node.get<float>(XlfResources::Region::Left));
    options.top = static_cast<int>(node.get<float>(XlfResources::Region::Top));
    options.zindex = node.get<int>(XlfResources::Region::Zindex, DefaultRegionZindex);
    options.loop = static_cast<RegionOptions::Loop>(node.get<bool>(XlfResources::Region::Loop, DefaultRegionLoop));

    return options;
}

std::unique_ptr<IRegion> RegionParser::createRegion(const RegionOptions& options,
                                                    const std::shared_ptr<IRegionView>& view)
{
    return std::make_unique<Region>(options.id, options.loop, view);
}

std::shared_ptr<IRegionView> RegionParser::createView(const RegionOptions& options)
{
    return std::make_shared<RegionView>(options.width, options.height);
}

void RegionParser::addMedia(IRegion& region, const ptree_node& regionNode)
{
    for (auto [nodeName, node] : regionNode)
    {
        if (nodeName != XlfResources::MediaNode) continue;

        auto&& parser = MediaCreatorsRepo::get<MediaParser>(mediaTypeFrom(node));
        if (parser)
        {
            int width = region.view()->width();
            int height = region.view()->height();

            auto media = parser->mediaFrom(node, width, height);
            auto [x, y] = mediaPositionInRegion(region, *media);

            region.addMedia(std::move(media), x, y);
        }
    }
}

MediaOptions::Type RegionParser::mediaTypeFrom(const ptree_node& node)
{
    auto type = node.get<std::string>(XlfResources::Media::Type);
    auto render = node.get<std::string>(XlfResources::Media::Render);

    return {type, render};
}

std::pair<int, int> RegionParser::mediaPositionInRegion(IRegion& region, IMedia& media)
{
    auto regionView = region.view();
    auto mediaView = media.view();
    if (regionView && mediaView)
    {
        GetMediaPosition positionCalc{regionView->width(), regionView->height()};
        int x = positionCalc.getMediaX(mediaView->width(), media.align());
        int y = positionCalc.getMediaY(mediaView->height(), media.valign());

        return {x, y};
    }

    return {DefaultXPos, DefaultYPos};
}
