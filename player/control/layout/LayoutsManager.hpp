#pragma once

#include "control/layout/IMainLayout.hpp"

#include <memory>
#include <vector>
#include <map>

class Scheduler;

using MainLayoutLoaded = sigc::signal<void(const std::shared_ptr<IOverlayLayout>&)>;
using OverlaysLoaded = sigc::signal<void(const std::vector<std::shared_ptr<IOverlayLayout>>&)>;

class LayoutsManager
{
public:
    LayoutsManager(Scheduler& scheduler);

    void fetchAllLayouts();
    void fetchMainLayout(int layoutId);
    void fetchOverlays(std::vector<int> layoutsId);

    MainLayoutLoaded& mainLayoutFetched();
    OverlaysLoaded& overlaysFetched();

private:
    template<typename LayoutLoader>
    std::unique_ptr<IMainLayout> createLayout(int layoutId);

private:
    std::unique_ptr<IMainLayout> m_mainLayout;
    std::map<int, std::unique_ptr<IMainLayout>> m_overlayLayouts;
    Scheduler& m_scheduler;

    MainLayoutLoaded m_mainLayoutFetched;
    OverlaysLoaded m_overlaysFetched;

};
