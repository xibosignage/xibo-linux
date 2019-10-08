#pragma once

#include "control/layout/MainLayout.hpp"

#include <map>
#include <memory>
#include <vector>

class Scheduler;
class FileCache;

using MainLayoutLoaded = boost::signals2::signal<void(const std::shared_ptr<Xibo::Widget>&)>;
using OverlaysLoaded = boost::signals2::signal<void(const std::vector<std::shared_ptr<Xibo::Widget>>&)>;

class LayoutsManager
{
public:
    LayoutsManager(Scheduler& scheduler, FileCache& fileCache);

    void fetchAllLayouts();
    void fetchMainLayout();
    void fetchOverlays();

    MainLayoutLoaded& mainLayoutFetched();
    OverlaysLoaded& overlaysFetched();

private:
    template <typename LayoutParser>
    std::unique_ptr<Xibo::MainLayout> createLayout(int layoutId);

private:
    std::unique_ptr<Xibo::MainLayout> mainLayout_;
    std::map<int, std::unique_ptr<Xibo::MainLayout>> overlayLayouts_;
    Scheduler& scheduler_;
    FileCache& fileCache_;

    MainLayoutLoaded mainLayoutFetched_;
    OverlaysLoaded overlaysFetched_;
};
