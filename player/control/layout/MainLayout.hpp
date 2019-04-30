#pragma once

#include "control/region/Region.hpp"
#include "MainLayoutOptions.hpp"
#include "MainLayoutView.hpp"

#include <vector>
#include <set>
#include <boost/noncopyable.hpp>
#include <sigc++/signal.h>

using SignalLayoutExpired = sigc::signal<void()>;

class MainLayout : private boost::noncopyable
{
public:
    MainLayout(const std::shared_ptr<MainLayoutView>& view);

    void addRegion(std::unique_ptr<Region>&& region, int left, int top, int zindex);
    SignalLayoutExpired expired();
    std::shared_ptr<MainLayoutView> view() const;

private:
    void onRegionExpired(int regionId);
    bool areAllRegionsExpired() const;

private:
    std::shared_ptr<MainLayoutView> m_view;
    std::vector<std::unique_ptr<Region>> m_regions;
    std::set<int> m_expiredRegions;
    SignalLayoutExpired m_layoutExpired;

};
