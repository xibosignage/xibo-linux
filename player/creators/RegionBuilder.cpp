#include "RegionBuilder.hpp"

#include "control/Region.hpp"
#include "utils/TimerProvider.hpp"
#include "media/IMedia.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

#include <boost/optional/optional.hpp>

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

std::unique_ptr<IRegion> RegionBuilder::build()
{
    auto region = createRegion();
    loopMedia(*region);
    addAllMedia(*region);
    return region;
}

std::unique_ptr<IRegion> RegionBuilder::createRegion()
{
    return std::make_unique<Region>(m_id, m_width, m_height, m_zorder, createTimer(), createAdaptor());
}

std::unique_ptr<IFixedLayoutAdaptor> RegionBuilder::createAdaptor()
{
    return std::make_unique<GtkFixedLayoutAdaptor>();
}

std::unique_ptr<ITimerProvider> RegionBuilder::createTimer()
{
    return std::make_unique<TimerProvider>();
}

void RegionBuilder::loopMedia(IRegion& region)
{
    if(m_loop)
    {
        region.loopMedia();
    }
}

void RegionBuilder::addAllMedia(IRegion& region)
{
    assert(m_visibleMedia.size() > 0 || m_invisibleMedia.size() > 0);

    for(auto&& ct : m_visibleMedia)
    {
        region.addMedia(std::move(ct.media), ct.x, ct.y);
    }

    for(auto&& media : m_invisibleMedia)
    {
        region.addMedia(std::move(media));
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

RegionBuilder& RegionBuilder::visibleMedia(std::vector<MediaWithPos>&& visibleMedia)
{
    m_visibleMedia = std::move(visibleMedia);
    return *this;
}

RegionBuilder& RegionBuilder::invisibleMedia(std::vector<std::unique_ptr<IMedia>>&& invisibleMedia)
{
    m_invisibleMedia = std::move(invisibleMedia);
    return *this;
}
