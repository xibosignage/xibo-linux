#include "MainLayout.hpp"

#include "IRegion.hpp"
#include "IBackground.hpp"

#include "adaptors/IImageAdaptor.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"
#include "adaptors/IOverlayAdaptor.hpp"

#include "events/Event.hpp"
#include "utils/logger/Logging.hpp"

#include <cassert>

MainLayout::MainLayout(int width, int height, std::unique_ptr<IOverlayAdaptor>&& handler) :
    m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->setSize(width, height);
}

MainLayout::~MainLayout()
{
}

int MainLayout::width() const
{
    return m_handler->width();
}

int MainLayout::height() const
{
    return m_handler->height();
}

void MainLayout::scale(double scaleX, double scaleY)
{
    assert(m_background);
    assert(m_regions.size() > 0);

    m_handler->scale(scaleX, scaleY);
    m_background->scale(scaleX, scaleY);
    scaleRegions(scaleX, scaleY);
}

void MainLayout::scaleRegions(double scaleX, double scaleY)
{
    for(auto&& region : m_regions)
    {
        region->scale(scaleX, scaleY);
    }
}

void MainLayout::addRegion(std::unique_ptr<IRegion>&& region, int x, int y)
{
    assert(region);

    checkRegionSize(region->width(), region->height());

    m_expiredRegions[region->id()] = false;
    region->subscribe(EventType::DurationExpired, std::bind(&MainLayout::handleEvent, this, std::placeholders::_1));
    m_handler->addChild(region->handler(), region->width(), region->height(), x, y);
    m_regions.push_back(std::move(region));
}

void MainLayout::handleEvent(const Event& ev)
{
    switch(ev.type())
    {
        case EventType::DurationExpired:
        {
            auto event = static_cast<const RegionDurationExpiredEvent&>(ev);
            onRegionExpired(event.id());
            break;
        }
        default:
            break;
    }
}

void MainLayout::onRegionExpired(int regionId)
{
    Log::debug("Region expired");
    m_expiredRegions[regionId] = true;

    if(areAllRegionsExpired())
    {
        pushEvent(DurationExpiredEvent{});
        Log::debug("Duration expired layout sent");
    }
}

bool MainLayout::areAllRegionsExpired() const
{
    bool allRegionsExpired = true;
    for(auto [id, expired] : m_expiredRegions)
    {
        if(!expired)
        {
            allRegionsExpired = false;
        }
    }
    return allRegionsExpired;
}

void MainLayout::checkRegionSize(int regionWidth, int regionHeight)
{
    if(regionWidth > width() || regionHeight > height())
        throw std::invalid_argument("Region width/height should not be greater than in layout");
}

IOverlayAdaptor& MainLayout::handler()
{
    return *m_handler;
}

void MainLayout::show()
{
    assert(m_background);
    assert(m_regions.size() > 0);

    if(!m_handler->isShown())
    {
        m_handler->show();
        m_background->show();
        sortReorderAndShowRegions();
    }
}

void MainLayout::sortReorderAndShowRegions()
{
    sortAndReorderRegions();

    for(auto&& region : m_regions)
    {
        region->show();
    }
}

void MainLayout::sortAndReorderRegions()
{
    sortRegionsByZorder();

    for(size_t i = 0; i != m_regions.size(); ++i)
    {
        int orderInParentWidget = static_cast<int>(i);
        m_handler->reorderChild(m_regions[i]->handler(), orderInParentWidget);

        Log::trace("Zorder: {} Order in overlay: {}", m_regions[i]->zorder(), orderInParentWidget);
    }
}

void MainLayout::sortRegionsByZorder()
{
    std::stable_sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first->zorder() < second->zorder();
    });
}

void MainLayout::setBackground(std::unique_ptr<IBackground>&& background)
{
    assert(background);

    checkBackgroundSize(background->width(), background->height());

    removePreviousBackgroundIfSet();
    m_background = std::move(background);
    m_handler->addMainChild(m_background->handler());
}

void MainLayout::checkBackgroundSize(int backgroundWidth, int backgroundHeight)
{
    if(backgroundWidth != width() || backgroundHeight != height())
        throw std::invalid_argument("Background's and layout's size should be the same");
}

void MainLayout::removePreviousBackgroundIfSet()
{
    if(m_background)
    {
        m_handler->removeMainChild();
    }
}
