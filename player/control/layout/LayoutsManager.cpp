#include "LayoutsManager.hpp"

#include "config.hpp"
#include "XlfLayoutFetcher.hpp"
#include "common/logger/Logging.hpp"

#include "control/common/Image.hpp"
#include "control/common/IOverlayLayout.hpp"

#include "schedule/Scheduler.hpp"

LayoutsManager::LayoutsManager(Scheduler& scheduler) :
    m_scheduler(scheduler)
{
}

void LayoutsManager::fetchAllLayouts()
{
    fetchMainLayout(m_scheduler.nextLayoutId());
    fetchOverlays(m_scheduler.nextOverlayLayoutsIds());
}

MainLayoutLoaded& LayoutsManager::mainLayoutFetched()
{
    return m_mainLayoutFetched;
}

OverlaysLoaded& LayoutsManager::overlaysFetched()
{
    return m_overlaysFetched;
}

void LayoutsManager::fetchMainLayout(int layoutId)
{
    if(layoutId != EmptyLayoutId)
    {
        m_mainLayout = createLayout<XlfMainLayoutLoader>(layoutId);
        m_mainLayoutFetched.emit(m_mainLayout->view());
    }
    else
    {
        m_mainLayoutFetched.emit(nullptr);
    }
}

void LayoutsManager::fetchOverlays(std::vector<int> layoutsId)
{
    if(layoutsId.empty()) return;

    std::vector<std::shared_ptr<IOverlayLayout>> overlays;

    m_overlayLayouts.clear();

    for(int id : layoutsId)
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
            fetchMainLayout(m_scheduler.nextLayoutId());
        }
        else
        {
            m_overlayLayouts[layoutId]->restart();
        }
    });

    return layout;
}

