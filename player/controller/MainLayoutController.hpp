#pragma once

#include "events/EventPublisher.hpp"

#include "model/MainLayoutModel.hpp"
#include "view/OverlayLayout.hpp"
#include "controller/RegionController.hpp"

#include <vector>
#include <set>
#include <boost/noncopyable.hpp>

class MainLayoutController : public EventPublisher<>, private boost::noncopyable
{
public:
    MainLayoutController(const std::shared_ptr<MainLayoutModel>& model,
                         const std::shared_ptr<OverlayLayout>& view);

private:
    void updateBackground();
    void addRegion(const std::shared_ptr<RegionModel>& regionModel);

    void onRegionExpired(const Event& ev);
    bool areAllRegionsExpired() const;

private:
    std::shared_ptr<MainLayoutModel> m_model;
    std::shared_ptr<OverlayLayout> m_view;
    std::vector<std::unique_ptr<RegionController>> m_regions;
    std::set<int> m_expiredRegions; // FIXME better to use 1 data structure

};
