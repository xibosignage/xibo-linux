#include "RegionBuilder.hpp"
#include "MediaFactory.hpp"

#include "parsers/RegionParser.hpp"
#include "parsers/ParserHelpers.hpp"
#include "control/Region.hpp"

int RegionBuilder::available_index = 0;

RegionBuilder::RegionBuilder(const ParsedRegion& params) :
    m_params(params)
{
}

std::unique_ptr<Region> RegionBuilder::build()
{
    auto region = create_from_params();

    for(auto media : m_params.media)
    {
        region->add_media(MediaFactory::create(*region, media));
    }
    return region;
}

RegionBuilder& RegionBuilder::set_width_scale_factor(double width_scale_factor)
{
    m_width_scale_factor = width_scale_factor;
    return *this;
}

RegionBuilder& RegionBuilder::set_height_scale_factor(double height_scale_factor)
{
    m_height_scale_factor = height_scale_factor;
    return *this;
}

std::unique_ptr<Region> RegionBuilder::create_from_params()
{
    int zindex = m_params.zindex.value_or(available_index++);
    bool loop = m_params.loop.value_or(false);

    int width = static_cast<int>(m_params.width * m_width_scale_factor);
    int height = static_cast<int>(m_params.height * m_height_scale_factor);
    Size sz{width, height};

    int top = static_cast<int>(m_params.left * m_width_scale_factor);
    int left = static_cast<int>(m_params.top * m_height_scale_factor);
    Point pos{top, left};

    return std::make_unique<Region>(m_params.id, sz, pos, zindex, loop);
}
