#include "MainLayoutImpl.hpp"

#include "control/widgets/FixedLayout.hpp"
#include "control/widgets/Image.hpp"
#include "control/widgets/OverlayLayoutFactory.hpp"

#include "common/logger/Logging.hpp"

namespace ph = std::placeholders;

MainLayoutImpl::MainLayoutImpl(const MainLayoutOptions& options) :
    options_(options),
    view_(OverlayLayoutFactory::create(options.width, options.height))
{
    if (options_.statEnabled)
    {
        view_->shown().connect([this]() {
            layoutStats_.id = options_.id;
            layoutStats_.started = DateTime::now();
        });
    }
}

void MainLayoutImpl::setBackground(std::shared_ptr<Xibo::Image>&& background)
{
    assert(view_);

    if (background)
    {
        view_->setMainChild(background);
    }
}

void MainLayoutImpl::addRegion(std::unique_ptr<Xibo::Region>&& region, int x, int y, int z)
{
    assert(view_);
    assert(region);
    assert(region->view());

    view_->addChild(region->view(), x, y, z);
    region->expired().connect(std::bind(&MainLayoutImpl::onRegionExpired, this, ph::_1));
    monitorMediaStats(*region);

    regions_.emplace_back(std::move(region));
}

void MainLayoutImpl::monitorMediaStats(Xibo::Region& region)
{
    for (auto&& media : region.mediaList())
    {
        auto statPolicy = media->statPolicy();
        if (statPolicy == MediaOptions::StatPolicy::Inherit)
        {
            media->enableStat(options_.statEnabled);
        }
        media->statReady().connect([this](const MediaStat& stat) { mediaStats_.emplace_back(std::move(stat)); });
    }
}

SignalLayoutExpired& MainLayoutImpl::expired()
{
    return layoutExpired_;
}

SignalLayoutStatReady& MainLayoutImpl::statReady()
{
    return statsReady_;
}

SignalLayoutMediaStatsReady& MainLayoutImpl::mediaStatsReady()
{
    return mediaStatsReady_;
}

// TODO view_->show() is not called here
void MainLayoutImpl::restart()
{
    layoutStats_.clear();
    mediaStats_.clear();
    expiredRegions_.clear();
    stopRegions();
    startRegions();
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
        stopRegions();
        if (options_.statEnabled)
        {
            layoutStats_.finished = DateTime::now();
            statsReady_(layoutStats_);
        }
        if (!mediaStats_.empty())
        {
            mediaStatsReady_(mediaStats_);
        }

        layoutExpired_();
    }
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
