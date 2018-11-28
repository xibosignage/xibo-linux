#include "MediaContainerBuilder.hpp"

#include "control/MediaContainer.hpp"
#include "utils/TimerProvider.hpp"
#include "media/IMedia.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

#include <boost/optional/optional.hpp>

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

std::unique_ptr<IMediaContainer> MediaContainerBuilder::build()
{
    auto container = createContainer();
    loopMedia(*container);
    addAllMedia(*container);
    return container;
}

std::unique_ptr<IMediaContainer> MediaContainerBuilder::createContainer()
{
    return std::make_unique<MediaContainer>(m_id, m_width, m_height, m_zorder, createTimer(), createAdaptor());
}

std::unique_ptr<IFixedLayoutAdaptor> MediaContainerBuilder::createAdaptor()
{
    return std::make_unique<GtkFixedLayoutAdaptor>();
}

std::unique_ptr<ITimerProvider> MediaContainerBuilder::createTimer()
{
    return std::make_unique<TimerProvider>();
}

void MediaContainerBuilder::loopMedia(IMediaContainer& container)
{
    if(m_loop)
    {
        container.loopMedia();
    }
}

void MediaContainerBuilder::addAllMedia(IMediaContainer& container)
{
    assert(m_visibleMedia.size() > 0 || m_invisibleMedia.size() > 0);

    for(auto&& ct : m_visibleMedia)
    {
        container.addMedia(std::move(ct.media), ct.x, ct.y);
    }

    for(auto&& media : m_invisibleMedia)
    {
        container.addMedia(std::move(media));
    }
}

MediaContainerBuilder& MediaContainerBuilder::id(int id)
{
    m_id = id;
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::width(int width)
{
    checkWidth(width);

    m_width = width;
    return *this;
}

void MediaContainerBuilder::checkWidth(int width)
{
    if(width <= MIN_WIDTH)
        throw std::invalid_argument("Width is too large/small");
}

MediaContainerBuilder& MediaContainerBuilder::height(int height)
{
    checkHeight(height);

    m_height = height;
    return *this;
}

void MediaContainerBuilder::checkHeight(int height)
{
    if(height < MIN_HEIGHT)
        throw std::invalid_argument("Height is too large/small");
}

MediaContainerBuilder& MediaContainerBuilder::zorder(const boost::optional<int>& zorder)
{
    m_zorder = zorder.value_or(DEFAULT_ZORDER);
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::loop(const boost::optional<bool>& loop)
{
    m_loop = loop.value_or(DEFAULT_LOOP);
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::visibleMedia(std::vector<MediaWithPos>&& visibleMedia)
{
    m_visibleMedia = std::move(visibleMedia);
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::invisibleMedia(std::vector<std::unique_ptr<IMedia>>&& invisibleMedia)
{
    m_invisibleMedia = std::move(invisibleMedia);
    return *this;
}
