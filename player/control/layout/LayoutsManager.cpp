#include "LayoutsManager.hpp"

#include "config.hpp"
#include "XlfLayoutFetcher.hpp"
#include "common/logger/Logging.hpp"

#include "control/common/Image.hpp"
#include "control/common/IOverlayLayout.hpp"

#include "schedule/XiboLayoutScheduler.hpp"

LayoutsManager::LayoutsManager(XiboLayoutScheduler& scheduler) :
    m_scheduler(scheduler)
{
}

void LayoutsManager::fetchAllLayouts()
{
    fetchMainLayout(m_scheduler.nextLayoutId());
    fetchOverlays(m_scheduler.nextOverlayLayoutsIds());
}

MainLayoutFetched& LayoutsManager::mainLayoutFetched()
{
    return m_mainLayoutFetched;
}

OverlaysFetched& LayoutsManager::overlaysFetched()
{
    return m_overlaysFetched;
}

void LayoutsManager::fetchMainLayout(int layoutId)
{
    if(layoutId != EmptyLayoutId)
    {
        m_mainLayout = createLayout<XlfMainLayoutFetcher>(layoutId);
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
        auto overlayLayout = createLayout<XlfOverlayLayoutFetcher>(id);
        overlays.emplace_back(overlayLayout->view());
        m_overlayLayouts.emplace(id, std::move(overlayLayout));
    }

    m_overlaysFetched.emit(overlays);
}

template<typename LayoutFetcher>
std::unique_ptr<IMainLayout> LayoutsManager::createLayout(int layoutId)
{
    auto layout = LayoutFetcher{}.fetch(layoutId);

    layout->expired().connect([this, layoutId](){
        Log::trace("Layout {} expired", layoutId);

        if constexpr(std::is_same_v<LayoutFetcher, XlfMainLayoutFetcher>)
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

