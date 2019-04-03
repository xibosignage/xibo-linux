#include "Region.hpp"

const int FIRST_CONTENT_INDEX = 0;

Region::Region(int id, RegionOptions::Loop looped) :
    m_id(id), m_looped(looped)
{
}

void Region::addMedia(std::unique_ptr<Media>&& media)
{
    media->mediaFinished().connect(std::bind(&Region::onMediaDurationTimeout, this));
    m_media.emplace_back(std::move(media));
}

void Region::start()
{
    placeMedia(FIRST_CONTENT_INDEX);
}

SignalRegionExpired Region::expired()
{
    return m_regionRexpired;
}

void Region::placeMedia(size_t mediaIndex)
{
    m_currentMediaIndex = mediaIndex;
    m_media[mediaIndex]->start();
}

void Region::removeMedia(size_t mediaIndex)
{
    m_media[mediaIndex]->stop();
}

void Region::onMediaDurationTimeout()
{
    if(isExpired())
    {
        m_regionRexpired.emit(m_id);
    }

    if(shouldBeMediaReplaced())
    {
        removeMedia(m_currentMediaIndex);
        placeMedia(getNextMediaIndex());
    }
}

bool Region::isExpired() const
{
    return getNextMediaIndex() == FIRST_CONTENT_INDEX;
}

bool Region::shouldBeMediaReplaced() const
{
    return m_media.size() > 1 || m_looped == RegionOptions::Loop::Enable;
}

size_t Region::getNextMediaIndex() const
{
    size_t nextContentIndex = m_currentMediaIndex + 1;

    if(nextContentIndex >= m_media.size())
        return FIRST_CONTENT_INDEX;

    return nextContentIndex;
}
