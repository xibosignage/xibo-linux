#include "MediaContainer.hpp"
#include "constants.hpp"

#include "media/IMedia.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

#include <cassert>

const int FIRST_MEDIA_INDEX = 0;

MediaContainer::MediaContainer(int zorder, bool looped, std::unique_ptr<IFixedLayoutAdaptor>&& handler) :
    m_handler(std::move(handler)), m_zorder(zorder), m_looped(looped)
{
    assert(m_handler);
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
    startCurrentMedia();
}

void MediaContainer::startCurrentMedia()
{
    m_media[m_currentMediaIndex]->start();
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

    media->connect(std::bind(&MediaContainer::onMediaTimeout, this));
    m_media.push_back(std::move(media));
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
