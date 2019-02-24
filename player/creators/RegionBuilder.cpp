#include "RegionBuilder.hpp"

#include "control/Region.hpp"
#include "control/IRegionContent.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

#include <boost/optional/optional.hpp>

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

RegionBuilder& RegionBuilder::retrieveOptions(const RegionOptions& opts)
{
    m_id = opts.id();
    m_width = getWidthOption(opts.width());
    m_height = getHeightOption(opts.height());
    m_zorder = getZorderOption(opts.zorder());
    m_loop = getLoopOption(opts.loop());
    return *this;
}

std::unique_ptr<IRegion> RegionBuilder::create()
{
    return std::unique_ptr<Region>(new Region{m_id, m_width, m_height, m_zorder, createHandler()});
}

std::unique_ptr<IFixedLayoutAdaptor> RegionBuilder::createDefaultHandler()
{
    return std::make_unique<GtkFixedLayoutAdaptor>();
}

void RegionBuilder::doSetup(IRegion& region)
{
    loopContent(region);
    addAllContent(region);
}

int RegionBuilder::getWidthOption(int width)
{
    checkWidth(width);

    return width;
}

void RegionBuilder::checkWidth(int width)
{
    if(width <= MIN_WIDTH)
        throw std::invalid_argument("Width is too large/small");
}

int RegionBuilder::getHeightOption(int height)
{
    checkHeight(height);

    return height;
}

void RegionBuilder::checkHeight(int height)
{
    if(height < MIN_HEIGHT)
        throw std::invalid_argument("Height is too large/small");
}

int RegionBuilder::getZorderOption(const boost::optional<int>& zorder)
{
    return zorder.value_or(DEFAULT_REGION_ZORDER);
}

RegionOptions::Loop RegionBuilder::getLoopOption(const boost::optional<RegionOptions::Loop>& loop)
{
    return loop.value_or(DEFAULT_REGION_LOOP);
}

RegionBuilder& RegionBuilder::content(std::vector<ContentWithPos>&& visibleMedia)
{
    m_content = std::move(visibleMedia);
    return *this;
}

void RegionBuilder::loopContent(IRegion& region)
{
    if(m_loop == RegionOptions::Loop::Enable)
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
