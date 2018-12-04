#include "Region.hpp"
#include "constants.hpp"

#include "media/IMedia.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"
#include "utils/ITimerProvider.hpp"

#include <cassert>

const int FIRST_MEDIA_INDEX = 0;

Region::Region(int id, int width, int height, int zorder, std::unique_ptr<ITimerProvider>&& timer, std::unique_ptr<IFixedLayoutAdaptor>&& handler) :
    m_handler(std::move(handler)), m_id(id), m_zorder(zorder)
{
    assert(m_handler);

    m_handler->setSize(width, height);
}

Region::~Region()
{
}

int Region::width() const
{
    return m_handler->width();
}

int Region::height() const
{
    return m_handler->height();
}

void Region::scale(double scaleX, double scaleY)
{
    assert(m_media.size() > 0);

    m_handler->scale(scaleX, scaleY);
    scaleVisibleMedia(scaleX, scaleY);
}

void Region::loopMedia()
{
    m_mediaLooped = true;
}

int Region::id() const
{
    return m_id;
}

void Region::scaleVisibleMedia(double scaleX, double scaleY)
{
    for(auto media : m_visibleMedia)
    {
        media->scale(scaleX, scaleY);
    }
}

int Region::zorder() const
{
    return m_zorder;
}

IFixedLayoutAdaptor& Region::handler()
{
    return *m_handler;
}

void Region::show()
{
    assert(m_media.size() > 0);

    m_handler->show();
    placeMedia(FIRST_MEDIA_INDEX);
}

void Region::placeMedia(size_t mediaIndex)
{
    m_currentMediaIndex = mediaIndex;
    auto&& media = m_media[mediaIndex];
    if(auto playable = dynamic_cast<IPlayable*>(media.get()))
    {
        playable->play();
    }
    else if(auto visible = dynamic_cast<IVisible*>(media.get()))
    {
        visible->show();
    }
}

void Region::removeMedia(size_t mediaIndex)
{
    auto&& media = m_media[mediaIndex];
    if(auto playable = dynamic_cast<IPlayable*>(media.get()))
    {
        playable->stop();
    }
    else if(auto visible = dynamic_cast<IVisible*>(media.get()))
    {
        visible->hide();
    }
}

void Region::addMedia(std::unique_ptr<IMedia>&& media, int x, int y)
{
    auto visibleMedia = dynamic_cast<IVisible*>(media.get());

    m_handler->addChild(visibleMedia->handler(), x, y);
    m_visibleMedia.push_back(visibleMedia);

    initAndAddMediaToList(std::move(media));
}

void Region::addMedia(std::unique_ptr<IMedia>&& media)
{
    initAndAddMediaToList(std::move(media));
}

void Region::initAndAddMediaToList(std::unique_ptr<IMedia>&& media)
{
    assert(media);

    media->connect(std::bind(&Region::onMediaDurationTimeout, this)); // TODO check that it could emit after media stopped
    m_media.push_back(std::move(media));
}

void Region::onMediaDurationTimeout()
{
    if(shouldBeMediaReplaced())
    {
        removeMedia(m_currentMediaIndex);
        placeMedia(getNextMediaIndex());
    }
}

bool Region::shouldBeMediaReplaced()
{
    return m_media.size() > 1 || m_mediaLooped;
}

size_t Region::getNextMediaIndex()
{
    size_t nextMediaIndex = m_currentMediaIndex + 1;

    if(nextMediaIndex >= m_media.size())
        return FIRST_MEDIA_INDEX;

    return nextMediaIndex;
}
