#include "MediaContainerBuilder.hpp"

#include "control/IMediaContainer.hpp"
#include "media/IMedia.hpp"

#include <boost/optional/optional.hpp>

const int DEFAULT_ZORDER = 0;
const bool DEFAULT_LOOP = false;

MediaContainerBuilder& MediaContainerBuilder::setWidth(int width)
{
    m_width = width;
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::setHeight(int height)
{
    m_height = height;
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::setZorder(const boost::optional<int>& zorder)
{
    m_zorder = zorder.value_or(DEFAULT_ZORDER);
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::setLoop(const boost::optional<bool>& loop)
{
    m_loop = loop.value_or(DEFAULT_LOOP);
    return *this;
}

MediaContainerBuilder& MediaContainerBuilder::setMedia(std::vector<MediaStruct>&& media)
{
    m_media = std::move(media);
    return *this;
}

void MediaContainerBuilder::prepareContainer(IMediaContainer& container)
{
    container.setSize(m_width, m_height);
    if(m_loop)
    {
        container.loopMediaInContainer();
    }
    for(auto&& ct : m_media)
    {
        if(ct.type != "audio")
            container.addMedia(std::move(ct.media), ct.x, ct.y);
        else
            container.addMedia(std::move(ct.media));
    }
}
