#pragma once

#include "control/layout/MainLayout.hpp"
#include "control/layout/MainLayoutOptions.hpp"
#include "control/region/Region.hpp"
#include "control/widgets/OverlayContainer.hpp"

#include <boost/noncopyable.hpp>
#include <set>
#include <vector>

class MainLayoutImpl : public Xibo::MainLayout, private boost::noncopyable
{
public:
    MainLayoutImpl(const MainLayoutOptions& options);

    void setBackground(std::shared_ptr<Xibo::Image>&& background) override;
    void addRegion(std::unique_ptr<Xibo::Region>&& region, int left, int top, int zorder) override;
    SignalLayoutExpired& expired() override;
    SignalLayoutStatReady& statReady() override;
    SignalLayoutMediaStatsReady& mediaStatsReady() override;
    void restart() override;
    std::shared_ptr<Xibo::Widget> view() override;
    int id() const override;

private:
    void monitorMediaStats(Xibo::Region& region);
    void onRegionExpired(int regionId);
    void onAllRegionsExpired();
    void startLayout();
    void stopLayout();
    bool areAllRegionsExpired() const;
    void startRegions();
    void stopRegions();

private:
    MainLayoutOptions options_;
    std::shared_ptr<Xibo::OverlayContainer> view_;

    Stats::PlayingTime interval_;
    MediaPlayingTime mediaIntervals_;
    SignalLayoutStatReady statsReady_;
    SignalLayoutMediaStatsReady mediaStatsReady_;

    std::vector<std::unique_ptr<Xibo::Region>> regions_;
    std::set<int> expiredRegions_;
    SignalLayoutExpired expired_;
};
