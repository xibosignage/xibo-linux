#include "LayoutsManager.hpp"
#include "XlfLayoutLoader.hpp"
#include "control/common/IOverlayLayout.hpp"

#include "schedule/Scheduler.hpp"
#include "common/logger/Logging.hpp"
#include "config.hpp"

LayoutsManager::LayoutsManager(Scheduler& scheduler) :
    m_scheduler(scheduler)
{
    m_scheduler.layoutUpdated().connect(std::bind(&LayoutsManager::fetchMainLayout, this));
    m_scheduler.overlaysUpdated().connect(std::bind(&LayoutsManager::fetchOverlays, this));
}

void LayoutsManager::fetchAllLayouts()
{
    fetchMainLayout();
    fetchOverlays();
}

MainLayoutLoaded& LayoutsManager::mainLayoutFetched()
{
    return m_mainLayoutFetched;
}

OverlaysLoaded& LayoutsManager::overlaysFetched()
{
    return m_overlaysFetched;
}

void LayoutsManager::fetchMainLayout()
{
    auto id = m_scheduler.nextLayout();

    if(id != EmptyLayoutId)
    {
        m_mainLayout = createLayout<XlfMainLayoutLoader>(id);
        m_mainLayoutFetched.emit(m_mainLayout->view());
    }
    else
    {
        m_mainLayoutFetched.emit(nullptr);
    }
}

void LayoutsManager::fetchOverlays()
{
    std::vector<std::shared_ptr<IOverlayLayout>> overlays;

    m_overlayLayouts.clear();

    for(int id : m_scheduler.overlayLayouts())
    {
        auto overlayLayout = createLayout<XlfOverlayLayoutLoader>(id);
        overlays.emplace_back(overlayLayout->view());
        m_overlayLayouts.emplace(id, std::move(overlayLayout));
    }

    m_overlaysFetched.emit(overlays);
}

template<typename LayoutLoader>
std::unique_ptr<IMainLayout> LayoutsManager::createLayout(int layoutId)
{
    auto layout = LayoutLoader::loadBy(layoutId);

    layout->expired().connect([this, layoutId](){
        Log::trace("Layout {} expired", layoutId);

        if constexpr(std::is_same_v<LayoutLoader, XlfMainLayoutLoader>)
        {
            fetchMainLayout();
        }
        else
        {
            m_overlayLayouts[layoutId]->restart();
        }
    });

    return layout;
}

