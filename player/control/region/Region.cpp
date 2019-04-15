#include "Region.hpp"

const int FIRST_CONTENT_INDEX = 0;

Region::Region(const RegionOptions& options) :
    m_options(options)
{
}

Region::~Region()
{
    for(auto&& media : m_media)
    {
        media->started().block();
        media->stopped().block();
        media->mediaFinished().block();
    }
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
    return m_regionExpired;
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
    if(shouldBeMediaReplaced())
    {
        removeMedia(m_currentMediaIndex);
        placeMedia(getNextMediaIndex());
    }

    if(isExpired())
    {
        m_regionExpired.emit(m_options.id);
    }
}

bool Region::isExpired() const
{
    return m_currentMediaIndex == FIRST_CONTENT_INDEX;
}

bool Region::shouldBeMediaReplaced() const
{
    return m_media.size() > 1 || m_options.looped == RegionOptions::Loop::Enable;
}

size_t Region::getNextMediaIndex() const
{
    size_t nextContentIndex = m_currentMediaIndex + 1;

    if(nextContentIndex >= m_media.size())
        return FIRST_CONTENT_INDEX;

    return nextContentIndex;
}
