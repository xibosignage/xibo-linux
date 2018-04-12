#include "RegionBuilder.hpp"
#include "MediaFactory.hpp"

#include "parsers/RegionParser.hpp"
#include "parsers/ParserHelpers.hpp"
#include "control/Region.hpp"

int RegionBuilder::available_index = 0;

std::unique_ptr<Region> RegionBuilder::create(const ParsedRegion& object)
{
    auto region = create_from_params(object);

    for(auto media : object.media)
    {
        region->add_media(MediaFactory::create(*region, media));
    }
    return region;
}


std::unique_ptr<Region> RegionBuilder::create_from_params(const ParsedRegion& object)
{
    int zindex = object.zindex.value_or(available_index++);
    bool loop = object.loop.value_or(false);

    Size sz{object.width, object.height};
    Point pos{object.left, object.top};

    return std::make_unique<Region>(object.id, sz, pos, zindex, loop);
}
