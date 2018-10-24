#include "MediaContainer.hpp"
#include "constants.hpp"

#include "media/IMedia.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

#include <cassert>

const int FIRST_MEDIA_INDEX = 0;

MediaContainer::MediaContainer(int width, int height, int zorder, bool mediaLooped, std::unique_ptr<ITimerProvider>&& timer, std::unique_ptr<IFixedLayoutAdaptor>&& handler) :
    m_handler(std::move(handler)), m_timer(std::move(timer)), m_zorder(zorder), m_mediaLooped(mediaLooped)
{
    assert(m_timer);
    assert(m_handler);

    m_handler->setSize(width, height);
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

void MediaContainer::scale(double scaleX, double scaleY)
{
    assert(m_media.size() > 0);

    m_handler->scale(scaleX, scaleY);
    scaleVisibleMedia(scaleX, scaleY);
}

void MediaContainer::scaleVisibleMedia(double scaleX, double scaleY)
{
    for(auto media : m_visibleMedia)
    {
        media->scale(scaleX, scaleY);
    }
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
    assert(m_media.size() > 0);

    m_handler->show();
    startMedia(FIRST_MEDIA_INDEX);
}

void MediaContainer::startMedia(size_t mediaIndex)
{
    auto&& media = m_media[mediaIndex];
    auto mediaDuration = static_cast<unsigned int>(media->duration()) * MSECS;
    m_currentMediaIndex = mediaIndex;

    if(mediaDuration > 0)
    {
        m_timer->startOnce(mediaDuration, std::bind(&MediaContainer::onMediaTimeout, this));
    }

    media->start();
}

void MediaContainer::addMedia(std::unique_ptr<IMedia>&& media, int x, int y)
{
    auto visibleMedia = dynamic_cast<IVisibleMedia*>(media.get());

    m_handler->addChild(visibleMedia->handler(), x, y);
    m_visibleMedia.push_back(visibleMedia);

    initAndAddMediaToList(std::move(media));
}

void MediaContainer::addMedia(std::unique_ptr<IMedia>&& media)
{
    initAndAddMediaToList(std::move(media));
}

void MediaContainer::initAndAddMediaToList(std::unique_ptr<IMedia>&& media)
{
    assert(media);

    media->connect(std::bind(&MediaContainer::onMediaTimeout, this)); // TODO check that it could emit after media stopped
    m_media.push_back(std::move(media));
}

void MediaContainer::onMediaTimeout()
{
    m_media[m_currentMediaIndex]->stop();

    if(shouldNextMediaStart())
    {
        startMedia(getNextMediaIndex());
    }
}

bool MediaContainer::shouldNextMediaStart()
{
    return m_media.size() > 1 || m_mediaLooped;
}

size_t MediaContainer::getNextMediaIndex()
{
    size_t nextMediaIndex = m_currentMediaIndex + 1;

    if(nextMediaIndex >= m_media.size())
        return FIRST_MEDIA_INDEX;

    return nextMediaIndex;
}
