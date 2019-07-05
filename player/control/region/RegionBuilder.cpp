#include "RegionBuilder.hpp"
#include "RegionView.hpp"
#include "Region.hpp"

#include "constants.hpp"

#include "control/media/GetMediaPosition.hpp"
#include "control/media/creators/MediaFactoriesRepo.hpp"
#include "control/media/creators/MediaFactory.hpp"

std::unique_ptr<IRegion> RegionBuilder::build(const ParsedRegion& parsedRegion)
{
    auto regionView = createView(parsedRegion.options);
    auto region = std::make_unique<Region>(parsedRegion.options.id, parsedRegion.options.loop, regionView);

    addMedia(*region, parsedRegion.media);

    return region;
}

std::shared_ptr<IRegionView> RegionBuilder::createView(const RegionOptions& options)
{
    return std::make_shared<RegionView>(options.width, options.height);
}

void RegionBuilder::addMedia(IRegion& region, const std::vector<ParsedMedia>& mediaList)
{
    for(auto&& parsedMedia : mediaList)
    {
        auto&& factory = MediaFactoriesRepo::get(parsedMedia.baseOptions.type);

        if(factory)
        {
            auto media = factory->create(parsedMedia);

            auto [x, y] = getMediaPosInRegion(region, *media);
            region.addMedia(std::move(media), x, y);
        }
    }
}

std::pair<int, int> RegionBuilder::getMediaPosInRegion(IRegion& region, IMedia& media)
{
    auto regionView = region.view();
    auto mediaView = media.view();
    if(regionView && mediaView)
    {
        GetMediaPosition positionCalc{regionView->width(), regionView->height()};
        int left = positionCalc.getMediaLeft(mediaView->width(), media.align());
        int top = positionCalc.getMediaTop(mediaView->height(), media.valign());

        return {left, top};
    }

    return {DefaultXPos, DefaultYPos};
}
