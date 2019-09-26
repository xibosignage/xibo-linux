#pragma once

#include "IMainLayout.hpp"

#include "control/common/IOverlayLayout.hpp"
#include "control/region/IRegion.hpp"

#include <boost/noncopyable.hpp>
#include <set>
#include <vector>

class MainLayout : public IMainLayout, private boost::noncopyable
{
public:
    MainLayout(const std::shared_ptr<IOverlayLayout>& view);

    void addRegion(std::unique_ptr<IRegion>&& region, int x, int y, int z) override;
    SignalLayoutExpired& expired() override;

    void restart() override;

    std::shared_ptr<IOverlayLayout> view() override;

private:
    void onRegionExpired(int regionId);
    bool areAllRegionsExpired() const;

private:
    std::shared_ptr<IOverlayLayout> view_;
    std::vector<std::unique_ptr<IRegion>> regions_;
    std::set<int> expiredRegions_;
    SignalLayoutExpired layoutExpired_;
};
