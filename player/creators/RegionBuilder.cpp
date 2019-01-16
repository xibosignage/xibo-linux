#include "RegionBuilder.hpp"

#include "control/Region.hpp"
#include "control/IRegionContent.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

#include <boost/optional/optional.hpp>

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

std::unique_ptr<IRegion> RegionBuilder::build()
{
    auto region = createRegion();
    loopContent(*region);
    addAllContent(*region);
    return region;
}

std::unique_ptr<IRegion> RegionBuilder::createRegion()
{
    return std::make_unique<Region>(m_id, m_width, m_height, m_zorder, createAdaptor());
}

std::unique_ptr<IFixedLayoutAdaptor> RegionBuilder::createAdaptor()
{
    return std::make_unique<GtkFixedLayoutAdaptor>();
}

void RegionBuilder::loopContent(IRegion& region)
{
    if(m_loop)
    {
        region.loopContent();
    }
}

void RegionBuilder::addAllContent(IRegion& region)
{
    assert(m_content.size() > 0);

    for(auto&& ct : m_content)
    {
        region.addContent(std::move(ct.content), ct.x, ct.y);
    }
}

RegionBuilder& RegionBuilder::id(int id)
{
    m_id = id;
    return *this;
}

RegionBuilder& RegionBuilder::width(int width)
{
    checkWidth(width);

    m_width = width;
    return *this;
}

void RegionBuilder::checkWidth(int width)
{
    if(width <= MIN_WIDTH)
        throw std::invalid_argument("Width is too large/small");
}

RegionBuilder& RegionBuilder::height(int height)
{
    checkHeight(height);

    m_height = height;
    return *this;
}

void RegionBuilder::checkHeight(int height)
{
    if(height < MIN_HEIGHT)
        throw std::invalid_argument("Height is too large/small");
}

RegionBuilder& RegionBuilder::zorder(const boost::optional<int>& zorder)
{
    m_zorder = zorder.value_or(DEFAULT_ZORDER);
    return *this;
}

RegionBuilder& RegionBuilder::loop(const boost::optional<bool>& loop)
{
    m_loop = loop.value_or(DEFAULT_LOOP);
    return *this;
}

RegionBuilder& RegionBuilder::content(std::vector<ContentWithPos>&& visibleMedia)
{
    m_content = std::move(visibleMedia);
    return *this;
}
