#include "RegionImpl.hpp"

#include "control/region/GetMediaPosition.hpp"
#include "control/widgets/FixedContainer.hpp"

RegionImpl::RegionImpl(const RegionOptions& options) :
    options_(options),
    view_(FixedContainerFactory::create(options.width, options.height)),
    currentMediaIndex_(FirstMediaIndex)
{
    assert(view_);

    view_->skipShowAll();
}

void RegionImpl::addMedia(std::unique_ptr<Xibo::Media>&& media)
{
    assert(media);

    media->finished().connect(std::bind(&RegionImpl::onMediaDurationTimeout, this));

    // Media can be invisible on the screen so we check if media has view
    auto mediaView = media->view();
    if (mediaView)
    {
        auto [x, y] = calcMediaPosition(*media);
        view_->add(mediaView, x, y, MediaOrder);
    }

    mediaList_.emplace_back(std::move(media));
}

std::pair<int, int> RegionImpl::calcMediaPosition(Xibo::Media& media)
{
    auto mediaView = media.view();

    GetMediaPosition positionCalc{view_->width(), view_->height()};
    int x = positionCalc.getMediaX(mediaView->width(), media.align());
    int y = positionCalc.getMediaY(mediaView->height(), media.valign());

    return {x, y};
}

void RegionImpl::start()
{
    assert(!mediaList_.empty());

    view_->show();
    placeMedia(FirstMediaIndex);
}

void RegionImpl::stop()
{
    for (auto&& media : mediaList_)
    {
        media->stop();
    }
    view_->hide();
}

SignalRegionExpired& RegionImpl::expired()
{
    return regionExpired_;
}

std::shared_ptr<Xibo::Widget> RegionImpl::view()
{
    return view_;
}

const MediaList& RegionImpl::mediaList() const
{
    return mediaList_;
}

void RegionImpl::placeMedia(size_t mediaIndex)
{
    currentMediaIndex_ = mediaIndex;
    mediaList_[mediaIndex]->start();
}

void RegionImpl::removeMedia(size_t mediaIndex)
{
    mediaList_[mediaIndex]->stop();
}

void RegionImpl::onMediaDurationTimeout()
{
    if (shouldBeMediaReplaced())
    {
        removeMedia(currentMediaIndex_);
        placeMedia(getNextMediaIndex());
    }

    if (isExpired())
    {
        regionExpired_(options_.id);
    }
}

// TODO: it's wrong to check the index
// it should be marked as expired explicitly
// check if it can be expired multiple times
bool RegionImpl::isExpired() const
{
    return currentMediaIndex_ == FirstMediaIndex;
}

bool RegionImpl::shouldBeMediaReplaced() const
{
    return mediaList_.size() > 1 || options_.loop == RegionOptions::Loop::Enable;
}

size_t RegionImpl::getNextMediaIndex() const
{
    size_t nextContentIndex = currentMediaIndex_ + 1;

    if (nextContentIndex >= mediaList_.size()) return FirstMediaIndex;

    return nextContentIndex;
}
