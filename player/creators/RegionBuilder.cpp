#include "RegionBuilder.hpp"
#include "MediaFactory.hpp"

#include "parsers/RegionParser.hpp"
#include "control/Region.hpp"

std::unique_ptr<Region> RegionBuilder::create(int region_id)
{
    RegionParser parser;
    auto params = parser.parse_region(region_id);
    auto region = std::make_unique<Region>(params.get<int>("id"),
                                           Size{params.get<int>("width"), params.get<int>("height")},
                                           Point{params.get<int>("left"), params.get<int>("top")},
                                           params.get<int>("zindex"),
                                           params.get<bool>("loop"));

    auto media = parser.media_ids();
    for(auto media_id : media)
    {
        region->add_media(MediaFactory::create(*region, media_id));
    }
    return region;
}
