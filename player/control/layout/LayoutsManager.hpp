#pragma once

#include "control/layout/MainLayout.hpp"

#include <map>
#include <memory>
#include <vector>

class Scheduler;
class FileCache;
class StatsRecorder;

using MainLayoutLoaded = boost::signals2::signal<void(const std::shared_ptr<Xibo::Widget>&)>;
using OverlaysLoaded = boost::signals2::signal<void(const std::vector<std::shared_ptr<Xibo::Widget>>&)>;

class LayoutsManager
{
public:
    LayoutsManager(Scheduler& scheduler, StatsRecorder& statsRecorder, FileCache& fileCache);

    void fetchMainLayout();
    void fetchOverlays();

    MainLayoutLoaded& mainLayoutFetched();
    OverlaysLoaded& overlaysFetched();

private:
    template <typename LayoutParser>
    std::unique_ptr<Xibo::MainLayout> createLayout(int layoutId);

private:
    Scheduler& scheduler_;
    StatsRecorder& statsRecorder_;
    FileCache& fileCache_;

    std::unique_ptr<Xibo::MainLayout> currentMainLayout_;
    std::map<int, std::unique_ptr<Xibo::MainLayout>> overlayLayouts_;

    MainLayoutLoaded mainLayoutFetched_;
    OverlaysLoaded overlaysFetched_;
};
