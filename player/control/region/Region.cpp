#include "Region.hpp"

const int FirstContentIndex = 0;

Region::Region(int id, RegionOptions::Loop loop, const std::shared_ptr<IRegionView>& view) :
    id_(id),
    loop_(loop),
    view_(view)
{
    view_->shown().connect(std::bind(&Region::start, this));
}

void Region::addMedia(std::unique_ptr<IMedia>&& media, int x, int y)
{
    media->mediaFinished().connect(std::bind(&Region::onMediaDurationTimeout, this));

    view_->addMedia(media->view(), x, y);
    media_.emplace_back(std::move(media));
}

void Region::start()
{
    placeMedia(FirstContentIndex);
}

SignalRegionExpired& Region::expired()
{
    return regionExpired_;
}

std::shared_ptr<IRegionView> Region::view()
{
    return view_;
}

void Region::placeMedia(size_t mediaIndex)
{
    currentMediaIndex_ = mediaIndex;
    media_[mediaIndex]->start();
}

void Region::removeMedia(size_t mediaIndex)
{
    media_[mediaIndex]->stop();
}

void Region::onMediaDurationTimeout()
{
    if (shouldBeMediaReplaced())
    {
        removeMedia(currentMediaIndex_);
        placeMedia(getNextMediaIndex());
    }

    if (isExpired())
    {
        regionExpired_(id_);
    }
}

bool Region::isExpired() const
{
    return currentMediaIndex_ == FirstContentIndex;
}

bool Region::shouldBeMediaReplaced() const
{
    return media_.size() > 1 || loop_ == RegionOptions::Loop::Enable;
}

size_t Region::getNextMediaIndex() const
{
    size_t nextContentIndex = currentMediaIndex_ + 1;

    if (nextContentIndex >= media_.size()) return FirstContentIndex;

    return nextContentIndex;
}
