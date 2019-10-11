#pragma once

#include "control/layout/MainLayout.hpp"
#include "control/layout/MainLayoutOptions.hpp"
#include "control/region/Region.hpp"
#include "control/widgets/OverlayLayout.hpp"

#include <boost/noncopyable.hpp>
#include <set>
#include <vector>

class MainLayoutImpl : public Xibo::MainLayout, private boost::noncopyable
{
public:
    MainLayoutImpl(const MainLayoutOptions& options);

    void setBackground(std::shared_ptr<Xibo::Image>&& background) override;
    void addRegion(std::unique_ptr<Xibo::Region>&& region, int x, int y, int z) override;
    SignalLayoutExpired& expired() override;
    void restart() override;
    std::shared_ptr<Xibo::Widget> view() override;

private:
    void onRegionExpired(int regionId);
    bool areAllRegionsExpired() const;

private:
    std::shared_ptr<Xibo::OverlayLayout> view_;
    std::vector<std::unique_ptr<Xibo::Region>> regions_;
    std::set<int> expiredRegions_;
    SignalLayoutExpired layoutExpired_;
};
