#pragma once

#include "control/region/Region.hpp"
#include "MainLayoutOptions.hpp"

#include <vector>
#include <set>
#include <boost/noncopyable.hpp>
#include <sigc++/signal.h>

using SignalLayoutExpired = sigc::signal<void()>;

class MainLayout : private boost::noncopyable
{
public:
    void addRegion(std::unique_ptr<Region>&& region);
    SignalLayoutExpired expired();

private:
    void onRegionExpired(int regionId);
    bool areAllRegionsExpired() const;

private:
    std::vector<std::unique_ptr<Region>> m_regions;
    std::set<int> m_expiredRegions;
    SignalLayoutExpired m_layoutExpired;

};
