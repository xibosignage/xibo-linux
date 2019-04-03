#include "MainLayout.hpp"

#include "utils/logger/Logging.hpp"

namespace ph = std::placeholders;

void MainLayout::addRegion(std::unique_ptr<Region>&& region)
{
    region->expired().connect(std::bind(&MainLayout::onRegionExpired, this, ph::_1));

    m_regions.emplace_back(std::move(region));
}

SignalLayoutExpired MainLayout::expired()
{
    return m_layoutExpired;
}

void MainLayout::onRegionExpired(int regionId)
{
    Log::debug("Region expired");
    m_expiredRegions.insert(regionId);

    if(areAllRegionsExpired())
    {
        m_layoutExpired.emit();
        Log::debug("Duration expired layout sent");
    }
}

bool MainLayout::areAllRegionsExpired() const
{
    return m_expiredRegions.size() == m_regions.size();
}
