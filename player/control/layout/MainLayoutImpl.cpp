#include "MainLayoutImpl.hpp"

#include "control/widgets/Image.hpp"
#include "control/widgets/OverlayContainer.hpp"

#include "common/logger/Logging.hpp"

namespace ph = std::placeholders;

MainLayoutImpl::MainLayoutImpl(const MainLayoutOptions& options) :
    options_(options), view_(OverlayContainerFactory::create(options.width, options.height))
{
    view_->shown().connect(std::bind(&MainLayoutImpl::startLayout, this));
}

void MainLayoutImpl::setBackground(std::shared_ptr<Xibo::Image>&& background)
{
    if (background)
    {
        view_->setMainChild(background);
    }
}

void MainLayoutImpl::addRegion(std::unique_ptr<Xibo::Region>&& region, int left, int top, int zorder)
{
    assert(region);
    assert(region->view());

    view_->add(region->view(), left, top, zorder);
    region->expired().connect(std::bind(&MainLayoutImpl::onRegionExpired, this, ph::_1));
    monitorMediaStats(*region);

    regions_.emplace_back(std::move(region));
}

void MainLayoutImpl::monitorMediaStats(Xibo::Region& region)
{
    for (auto&& media : region.mediaList())
    {
        media->statReady().connect(
            [id = media->id(), this](const Stats::PlayingTime& interval) { mediaIntervals_.emplace(id, interval); });
    }
}

SignalLayoutExpired& MainLayoutImpl::expired()
{
    return expired_;
}

SignalLayoutStatReady& MainLayoutImpl::statReady()
{
    return statsReady_;
}

SignalLayoutMediaStatsReady& MainLayoutImpl::mediaStatsReady()
{
    return mediaStatsReady_;
}

void MainLayoutImpl::restart()
{
    stopLayout();

    interval_.clear();
    mediaIntervals_.clear();
    expiredRegions_.clear();

    startLayout();
}

std::shared_ptr<Xibo::Widget> MainLayoutImpl::view()
{
    return view_;
}

int MainLayoutImpl::id() const
{
    return options_.id;
}

void MainLayoutImpl::onRegionExpired(int regionId)
{
    Log::trace("[MainLayout] Region {} expired", regionId);
    expiredRegions_.insert(regionId);

    if (areAllRegionsExpired())
    {
        onAllRegionsExpired();
    }
}

void MainLayoutImpl::onAllRegionsExpired()
{
    stopLayout();
    if (options_.statEnabled)
    {
        statsReady_(interval_);
    }
    if (!mediaIntervals_.empty())
    {
        mediaStatsReady_(mediaIntervals_);
    }

    expired_();
}

void MainLayoutImpl::startLayout()
{
    interval_.started = DateTime::now();
    startRegions();
}

void MainLayoutImpl::stopLayout()
{
    interval_.finished = DateTime::now();
    stopRegions();
}

bool MainLayoutImpl::areAllRegionsExpired() const
{
    return expiredRegions_.size() == regions_.size();
}

void MainLayoutImpl::startRegions()
{
    for (auto&& region : regions_)
    {
        region->start();
    }
}

void MainLayoutImpl::stopRegions()
{
    for (auto&& region : regions_)
    {
        region->stop();
    }
}
