#include "RegionBuilder.hpp"
#include "MediaFactory.hpp"
#include "control/Region.hpp"

std::unique_ptr<Region> RegionBuilder::create(const Params& params)
{
    auto region = std::make_unique<Region>(params.get<int>("id"),
                                           Size{params.get<int>("width"), params.get<int>("height")},
                                           Point{params.get<int>("left"), params.get<int>("top")},
                                           params.get<int>("zindex"),
                                           params.get<bool>("loop"));

    auto media = params.get_child("medias");
    for(auto [media_id, media_params] : media)
    {
        region->add_media(MediaFactory::create(*region, media_params));
    }
    return region;
}
