#include "MainLayoutImpl.hpp"

#include "control/widgets/FixedLayout.hpp"
#include "control/widgets/Image.hpp"
#include "control/widgets/OverlayLayoutFactory.hpp"

#include "common/logger/Logging.hpp"

namespace ph = std::placeholders;

MainLayoutImpl::MainLayoutImpl(const MainLayoutOptions& options) :
    view_(OverlayLayoutFactory::create(options.width, options.height))
{
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

    regions_.emplace_back(std::move(region));
}

SignalLayoutExpired& MainLayoutImpl::expired()
{
    return layoutExpired_;
}

void MainLayoutImpl::restart()
{
    expiredRegions_.clear();

    for (auto&& region : regions_)
    {
        region->start();
    }
}

std::shared_ptr<Xibo::Widget> MainLayoutImpl::view()
{
    return view_;
}

void MainLayoutImpl::onRegionExpired(int regionId)
{
    Log::trace("[MainLayout] Region {} expired", regionId);
    expiredRegions_.insert(regionId);

    if (areAllRegionsExpired())
    {
        layoutExpired_();
    }
}

bool MainLayoutImpl::areAllRegionsExpired() const
{
    return expiredRegions_.size() == regions_.size();
}
