#include "MediaContainerBuilder.hpp"
#include "constants.hpp"

#include "control/MediaContainer.hpp"

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

std::unique_ptr<IMediaContainer> MediaContainerBuilder::build()
{
    assert(m_adaptor);

    m_adaptor->setSize(m_width, m_height);

    auto container = createContainer(m_zorder, m_loop);
    addAllMedia(*container);
    return container;
}

std::unique_ptr<IMediaContainer> MediaContainerBuilder::createContainer(int zorder, bool loop)
{
    return std::make_unique<MediaContainer>(zorder, loop, std::move(m_adaptor));
}

MediaContainerBuilder& MediaContainerBuilder::adaptor(std::unique_ptr<IFixedLayoutAdaptor>&& adaptor)
{
    m_adaptor = std::move(adaptor);
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
    if(width <= MIN_WIDTH || width >= MAX_DISPLAY_WIDTH)
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
    if(height < MIN_HEIGHT || height >= MAX_DISPLAY_HEIGHT)
        throw std::invalid_argument("Height is too large/small");
}

// FIXME it should not be default zorder
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

MediaContainerBuilder& MediaContainerBuilder::media(std::vector<MediaStruct>&& media)
{
    m_media = std::move(media);
    return *this;
}

void MediaContainerBuilder::addAllMedia(IMediaContainer& container)
{
    assert(m_media.size() > 0);

    for(auto&& ct : m_media)
    {
        if(ct.type != "audio")
            container.addMedia(std::move(ct.media), ct.x, ct.y);
        else
            container.addMedia(std::move(ct.media));
    }
}
