#include "MainLayout.hpp"

#include "common/logger/Logging.hpp"

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
        Log::debug("Duration expired layout sent");
        m_layoutExpired.emit();
    }
}

bool MainLayout::areAllRegionsExpired() const
{
    return m_expiredRegions.size() == m_regions.size();
}

