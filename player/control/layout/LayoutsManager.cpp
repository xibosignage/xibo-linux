#include "LayoutsManager.hpp"

#include "control/layout/MainLayoutParser.hpp"
#include "control/layout/OverlayLayoutParser.hpp"
#include "control/widgets/Widget.hpp"

#include "common/fs/FileCache.hpp"
#include "common/logger/Logging.hpp"
#include "managers/StatsRecorder.hpp"
#include "schedule/Scheduler.hpp"

#include "config.hpp"

LayoutsManager::LayoutsManager(Scheduler& scheduler, FileCache& fileCache) :
    scheduler_(scheduler),
    fileCache_(fileCache)
{
    scheduler_.layoutUpdated().connect(std::bind(&LayoutsManager::fetchMainLayout, this));
    scheduler_.overlaysUpdated().connect(std::bind(&LayoutsManager::fetchOverlays, this));
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
        mainLayout_ = createLayout<MainLayoutParser>(id);
        if (mainLayout_)
        {
            mainLayoutFetched_(mainLayout_->view());
        }
        else
        {
            fileCache_.markAsInvalid(std::to_string(id) + ".xlf");
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
    std::vector<std::shared_ptr<Xibo::Widget>> overlays;

    overlayLayouts_.clear();

    for (int id : scheduler_.overlayLayouts())
    {
        auto overlayLayout = createLayout<OverlayLayoutParser>(id);
        if (overlayLayout)
        {
            overlays.emplace_back(overlayLayout->view());
            overlayLayouts_.emplace(id, std::move(overlayLayout));
        }
        else
        {
            fileCache_.markAsInvalid(std::to_string(id) + ".xlf");
            scheduler_.reloadQueue();
        }
    }

    overlaysFetched_(overlays);
}

template <typename LayoutParser>
std::unique_ptr<Xibo::MainLayout> LayoutsManager::createLayout(int layoutId)
{
    try
    {
        LayoutParser parser;
        auto layout = parser.parseBy(layoutId);
        auto scheduleId = scheduler_.scheduleIdBy(layoutId);

        layout->statReady().connect(
            [scheduleId](const LayoutStat& stat) { StatsRecorder::get().addLayoutStat(scheduleId, stat); });
        layout->mediaStatsReady().connect([layoutId, scheduleId](const auto& stats) {
            StatsRecorder::get().addMediaStats(layoutId, scheduleId, stats);
        });

        layout->expired().connect([this, layoutId]() {
            Log::trace("[LayoutsManager] Layout {} expired", layoutId);

            if constexpr (std::is_same_v<LayoutParser, MainLayoutParser>)
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
        Log::error("[LayoutsManager] {}", e.what());
        Log::info("[LayoutsManager] Check resource folder to find out what happened");
    }

    return nullptr;
}
