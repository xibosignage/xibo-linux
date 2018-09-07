#include "MediaContainer.hpp"

#include "constants.hpp"

#include "media/IMedia.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

MediaContainer::MediaContainer(int id, int zorder, bool looped) :
    MediaContainer(id, zorder, looped, std::make_unique<GtkFixedLayoutAdaptor>())
{
}

MediaContainer::MediaContainer(int id, int zorder, bool looped,
                               std::unique_ptr<IFixedLayoutAdaptor> handler) :
    m_handler(std::move(handler)), m_id(id), m_zorder(zorder), m_looped(looped)
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
    m_handler->setSize(width, height);
}

int MediaContainer::id() const
{
    return m_id;
}

int MediaContainer::zorder() const
{
    return m_zorder;
}

void MediaContainer::show()
{
    if(!m_media.empty())
    {
        m_handler->show();
        m_media[m_currentMediaIndex]->start();
    }
}

IFixedLayoutAdaptor& MediaContainer::handler()
{
    return *m_handler;
}

void MediaContainer::addMedia(std::unique_ptr<IMedia> media, int x, int y)
{
    m_handler->addChild(media->handler(), x, y);
    initAndAddMediaToList(std::move(media));
}

void MediaContainer::addMedia(std::unique_ptr<IMedia> media)
{
    initAndAddMediaToList(std::move(media));
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
