#include "Region.hpp"

#include "control/media/GetMediaPosition.hpp"

const int FIRST_CONTENT_INDEX = 0;

Region::Region(const RegionOptions& options, const std::shared_ptr<RegionView>& view) :
    m_options(options),
    m_view(view)
{
    m_view->shown().connect(sigc::mem_fun(this, &Region::start));
}

void Region::addMedia(std::unique_ptr<Media>&& media)
{
    media->mediaFinished().connect(std::bind(&Region::onMediaDurationTimeout, this));
    auto mediaView = media->view();

    if(mediaView)
    {
        GetMediaPosition positionCalc{m_view->width(), m_view->height()};
        int left = positionCalc.getMediaLeft(mediaView->width(), media->align());
        int top = positionCalc.getMediaTop(mediaView->height(), media->valign());

        m_view->addMedia(mediaView, left, top);
    }

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

std::shared_ptr<RegionView> Region::view() const
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
