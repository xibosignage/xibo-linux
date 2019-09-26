#include "MainLayout.hpp"

#include "common/logger/Logging.hpp"
#include "control/region/IRegionView.hpp"

namespace ph = std::placeholders;

MainLayout::MainLayout(const std::shared_ptr<IOverlayLayout>& view) : view_(view) {}

void MainLayout::addRegion(std::unique_ptr<IRegion>&& region, int x, int y, int z)
{
    region->expired().connect(std::bind(&MainLayout::onRegionExpired, this, ph::_1));

    view_->addChild(region->view(), x, y, z);
    regions_.emplace_back(std::move(region));
}

SignalLayoutExpired& MainLayout::expired()
{
    return layoutExpired_;
}

void MainLayout::restart()
{
    expiredRegions_.clear();

    for (auto&& region : regions_)
    {
        region->start();
    }
}

std::shared_ptr<IOverlayLayout> MainLayout::view()
{
    return view_;
}

void MainLayout::onRegionExpired(int regionId)
{
    Log::trace("Region {} expired", regionId);
    expiredRegions_.insert(regionId);

    if (areAllRegionsExpired())
    {
        layoutExpired_();
    }
}

bool MainLayout::areAllRegionsExpired() const
{
    return expiredRegions_.size() == regions_.size();
}
