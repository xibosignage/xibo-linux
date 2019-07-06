#pragma once

#include "control/layout/IMainLayout.hpp"

#include <memory>
#include <vector>
#include <map>

class XiboLayoutScheduler;

using MainLayoutFetched = sigc::signal<void(const std::shared_ptr<IOverlayLayout>&)>;
using OverlaysFetched = sigc::signal<void(const std::vector<std::shared_ptr<IOverlayLayout>>&)>;

class LayoutsManager
{
public:
    LayoutsManager(XiboLayoutScheduler& scheduler);

    void fetchAllLayouts();
    void fetchMainLayout(int layoutId);
    void fetchOverlays(std::vector<int> layoutsId);

    MainLayoutFetched& mainLayoutFetched();
    OverlaysFetched& overlaysFetched();

private:
    template<typename LayoutFetcher>
    std::unique_ptr<IMainLayout> createLayout(int layoutId);

private:
    std::unique_ptr<IMainLayout> m_mainLayout;
    std::map<int, std::unique_ptr<IMainLayout>> m_overlayLayouts;
    XiboLayoutScheduler& m_scheduler;

    MainLayoutFetched m_mainLayoutFetched;
    OverlaysFetched m_overlaysFetched;

};
