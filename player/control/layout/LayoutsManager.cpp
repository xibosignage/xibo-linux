#include "LayoutsManager.hpp"
#include "XlfLayoutLoader.hpp"
#include "control/common/IOverlayLayout.hpp"

#include "common/logger/Logging.hpp"
#include "config.hpp"
#include "schedule/Scheduler.hpp"

#include "common/fs/FileCache.hpp"
#include "utils/Managers.hpp"

LayoutsManager::LayoutsManager(Scheduler& scheduler) : scheduler_(scheduler)
{
    scheduler_.layoutUpdated().connect(std::bind(&LayoutsManager::fetchMainLayout, this));
    scheduler_.overlaysUpdated().connect(std::bind(&LayoutsManager::fetchOverlays, this));
}

void LayoutsManager::fetchAllLayouts()
{
    fetchMainLayout();
    fetchOverlays();
}

MainLayoutLoaded& LayoutsManager::mainLayoutFetched()
{
    return mainLayoutFetched_;
}

OverlaysLoaded& LayoutsManager::overlaysFetched()
{
    return overlaysFetched_;
}

void LayoutsManager::fetchMainLayout()
{
    auto id = scheduler_.nextLayout();

    if (id != EmptyLayoutId)
    {
        mainLayout_ = createLayout<XlfMainLayoutLoader>(id);
        if (mainLayout_)
        {
            mainLayoutFetched_(mainLayout_->view());
        }
        else
        {
            Managers::fileManager().markAsInvalid(std::to_string(id) + ".xlf");
            scheduler_.reloadQueue();
            mainLayoutFetched_(nullptr);
        }
    }
    else
    {
        mainLayoutFetched_(nullptr);
    }
}

void LayoutsManager::fetchOverlays()
{
    std::vector<std::shared_ptr<IOverlayLayout>> overlays;

    overlayLayouts_.clear();

    for (int id : scheduler_.overlayLayouts())
    {
        auto overlayLayout = createLayout<XlfOverlayLayoutLoader>(id);
        if (overlayLayout)
        {
            overlays.emplace_back(overlayLayout->view());
            overlayLayouts_.emplace(id, std::move(overlayLayout));
        }
        else
        {
            Managers::fileManager().markAsInvalid(std::to_string(id) + ".xlf");
            scheduler_.reloadQueue();
        }
    }

    overlaysFetched_(overlays);
}

template <typename LayoutLoader>
std::unique_ptr<IMainLayout> LayoutsManager::createLayout(int layoutId)
{
    try
    {
        auto layout = LayoutLoader::loadBy(layoutId);

        layout->expired().connect([this, layoutId]() {
            Log::trace("Layout {} expired", layoutId);

            if constexpr (std::is_same_v<LayoutLoader, XlfMainLayoutLoader>)
            {
                fetchMainLayout();
            }
            else
            {
                overlayLayouts_[layoutId]->restart();
            }
        });

        return layout;
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
        Log::info("Check resource folder to find out what happened");
    }

    return nullptr;
}
