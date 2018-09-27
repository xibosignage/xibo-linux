#include "MediaContainer.hpp"
#include "constants.hpp"

#include "media/IMedia.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

#include <cassert>

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;
const int FIRST_MEDIA_INDEX = 0;

MediaContainer::MediaContainer(int zorder, std::unique_ptr<IFixedLayoutAdaptor>&& handler) :
    m_handler(std::move(handler)), m_zorder(zorder)
{
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
    checkContainerNewSize(width, height);

    m_handler->setSize(width, height);
}

void MediaContainer::checkContainerNewSize(int width, int height)
{
    if(width <= MIN_WIDTH || width >= MAX_DISPLAY_WIDTH || height < MIN_HEIGHT || height >= MAX_DISPLAY_HEIGHT)
        throw std::invalid_argument("Width/height is too large/small");
}

void MediaContainer::loopMediaInContainer()
{
    m_looped = true;
}

int MediaContainer::zorder() const
{
    return m_zorder;
}

IFixedLayoutAdaptor& MediaContainer::handler()
{
    return *m_handler;
}

void MediaContainer::show()
{
    m_handler->show();
    showCurrentMedia();
}

void MediaContainer::showCurrentMedia()
{
    if(!m_media.empty())
    {
        m_media[m_currentMediaIndex]->start();
    }
}

void MediaContainer::removeAllMedia()
{
    m_handler->removeChildren();
}

void MediaContainer::addMedia(std::unique_ptr<IMedia>&& media, int x, int y)
{
    auto&& addedMedia = initAndAddMediaToList(std::move(media));
    m_handler->addChild(addedMedia.handler(), x, y);
}

void MediaContainer::addMedia(std::unique_ptr<IMedia>&& media)
{
    initAndAddMediaToList(std::move(media));
}

IMedia& MediaContainer::initAndAddMediaToList(std::unique_ptr<IMedia>&& media)
{
    assert(media);

    media->connect(std::bind(&MediaContainer::onMediaTimeout, this));
    m_media.push_back(std::move(media));
    return *m_media.back();
}

// TODO Refactor after Media unit-tests
void MediaContainer::onMediaTimeout()
{
    if(m_media.size() > 1)
    {
        m_media[m_currentMediaIndex]->stop();
        m_currentMediaIndex = m_currentMediaIndex + 1 >= m_media.size() ? FIRST_MEDIA_INDEX : m_currentMediaIndex + 1;
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
