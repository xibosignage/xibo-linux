#include "MainLayoutController.hpp"

#include "utils/logger/Logging.hpp"
#include "view/Image.hpp"

#include <cassert>

namespace ph = std::placeholders;

MainLayoutController::MainLayoutController(const std::shared_ptr<MainLayoutModel>& model,
                                           const std::shared_ptr<MainLayoutView>& view) :
    m_model(model), m_view(view)
{
    updateBackground();
    for(auto&& regionModel : m_model->regions())
    {
        addRegion(regionModel);
    }
}

void MainLayoutController::updateBackground()
{
    auto background = std::make_shared<Image>(m_model->width(), m_model->height());

    if(m_model->backgroundUri().isValid())
        background->loadFromFile(m_model->backgroundUri(), true);
    else
        background->setColor(m_model->backgroundColor());

    m_view->addBackground(background);
}

void MainLayoutController::addRegion(const std::shared_ptr<RegionModel>& regionModel)
{
    auto regionView = std::make_shared<RegionView>(regionModel->width(), regionModel->height());
    auto regionController = std::make_unique<RegionController>(regionModel, regionView);

    regionController->subscribe(EventType::DurationExpired, std::bind(&MainLayoutController::onRegionExpired, this, ph::_1));

    m_view->addRegion(regionView, regionModel->left(), regionModel->top(), regionModel->zindex());
    m_regions.emplace_back(std::move(regionController));
}

void MainLayoutController::onRegionExpired(const Event& ev)
{
    auto regionExpiredEvent = static_cast<const RegionDurationExpiredEvent&>(ev);

    Log::debug("Region expired");
    m_expiredRegions.insert(regionExpiredEvent.id());

    if(areAllRegionsExpired())
    {
        pushEvent(DurationExpiredEvent{});
        Log::debug("Duration expired layout sent");
    }
}

bool MainLayoutController::areAllRegionsExpired() const
{
    return m_expiredRegions.size() == m_regions.size();
}
