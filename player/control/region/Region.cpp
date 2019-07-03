#include "Region.hpp"

const int FIRST_CONTENT_INDEX = 0;

Region::Region(int id, RegionOptions::Loop loop, const std::shared_ptr<IRegionView>& view) :
    m_id(id),
    m_loop(loop),
    m_view(view)
{
    m_view->shown().connect(sigc::mem_fun(this, &Region::start));
}

#include "common/logger/Logging.hpp"

void Region::addMedia(std::unique_ptr<IMedia>&& media, int x, int y)
{
    media->mediaFinished().connect(std::bind(&Region::onMediaDurationTimeout, this));

    m_view->addMedia(media->view(), x, y);
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

std::shared_ptr<IRegionView> Region::view()
{
    return m_view;
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
        m_regionExpired.emit(m_id);
    }
}

bool Region::isExpired() const
{
    return m_currentMediaIndex == FIRST_CONTENT_INDEX;
}

bool Region::shouldBeMediaReplaced() const
{
    return m_media.size() > 1 || m_loop == RegionOptions::Loop::Enable;
}

size_t Region::getNextMediaIndex() const
{
    size_t nextContentIndex = m_currentMediaIndex + 1;

    if(nextContentIndex >= m_media.size())
        return FIRST_CONTENT_INDEX;

    return nextContentIndex;
}
