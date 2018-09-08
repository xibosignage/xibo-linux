#include "MediaContainer.hpp"

#include "constants.hpp"

#include "media/IMedia.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

MediaContainer::MediaContainer(int width, int height, int zorder) :
    MediaContainer(width, height, zorder, std::make_unique<GtkFixedLayoutAdaptor>())
{
}

MediaContainer::MediaContainer(int width, int height, int zorder, std::unique_ptr<IFixedLayoutAdaptor> handler) :
    m_handler(std::move(handler)), m_zorder(zorder)
{
    setSize(width, height);
}

MediaContainer::~MediaContainer()
{
}

int MediaContainer::width() const
{
    return m_handler->width();
}

int MediaContainer::height() const
{
    return m_handler->height();
}

void MediaContainer::setSize(int width, int height)
{
    if(width <= MIN_WIDTH || width >= MAX_DISPLAY_WIDTH || height < MIN_HEIGHT || height >= MAX_DISPLAY_HEIGHT)
        throw std::runtime_error("Width/height is too large/small");

    m_handler->setSize(width, height);
}

void MediaContainer::loopMediaInContainer()
{
    m_looped = true;
}

int MediaContainer::zorder() const
{
    return m_zorder;
}

void MediaContainer::show()
{
    m_handler->show();
    if(!m_media.empty())
    {
        m_media[m_currentMediaIndex]->start();
    }
}

IFixedLayoutAdaptor& MediaContainer::handler()
{
    return *m_handler;
}

void MediaContainer::addMedia(std::unique_ptr<IMedia> media, int x, int y)
{
    if(!media)
        throw std::runtime_error("Invalid media given");

    m_handler->addChild(media->handler(), x, y);
    initAndAddMediaToList(std::move(media));
}

void MediaContainer::addMedia(std::unique_ptr<IMedia> media)
{
    if(!media)
        throw std::runtime_error("Invalid media given");

    initAndAddMediaToList(std::move(media));
}

void MediaContainer::removeAllMedia()
{
    m_handler->removeChildren();
}

void MediaContainer::initAndAddMediaToList(std::unique_ptr<IMedia> media)
{
    media->connect(std::bind(&MediaContainer::onMediaTimeout, this));
    m_media.push_back(std::move(media));
}

void MediaContainer::onMediaTimeout()
{
    if(m_media.size() > 1)
    {
        m_media[m_currentMediaIndex]->stop();
        m_currentMediaIndex = m_currentMediaIndex + 1 >= m_media.size() ? 0 : m_currentMediaIndex + 1;
        m_media[m_currentMediaIndex]->start();
    }
    else
    {
        if(m_looped)
        {
            m_media[m_currentMediaIndex]->stop();
            m_media[m_currentMediaIndex]->start();
        }
    }
}
