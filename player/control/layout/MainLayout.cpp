#include "MainLayout.hpp"

#include "common/logger/Logging.hpp"
#include "control/region/IRegionView.hpp"

namespace ph = std::placeholders;

MainLayout::MainLayout(const std::shared_ptr<IOverlayLayout>& view) : m_view(view) {}

void MainLayout::addRegion(std::unique_ptr<IRegion>&& region, int x, int y, int z)
{
    region->expired().connect(std::bind(&MainLayout::onRegionExpired, this, ph::_1));

    m_view->addChild(region->view(), x, y, z);
    m_regions.emplace_back(std::move(region));
}

SignalLayoutExpired& MainLayout::expired()
{
    return m_layoutExpired;
}

void MainLayout::restart()
{
    m_expiredRegions.clear();

    for (auto&& region : m_regions)
    {
        region->start();
    }
}

std::shared_ptr<IOverlayLayout> MainLayout::view()
{
    return m_view;
}

void MainLayout::onRegionExpired(int regionId)
{
    Log::trace("Region {} expired", regionId);
    m_expiredRegions.insert(regionId);

    if (areAllRegionsExpired())
    {
        m_layoutExpired();
    }
}

bool MainLayout::areAllRegionsExpired() const
{
    return m_expiredRegions.size() == m_regions.size();
}
