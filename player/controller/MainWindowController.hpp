#pragma once

#include "view/MainWindow.hpp"
#include "controller/StatusScreenController.hpp"
#include "controller/MainLayoutController.hpp"

class LayoutScheduler;

class MainWindowController
{
public:
    MainWindowController(std::shared_ptr<MainWindow> window, LayoutScheduler& scheduler);
    void updateLayout(int layoutId);

private:
    void removePreviousLayout();
    void scaleLayout(const std::shared_ptr<OverlayLayout>& layout);

private:
    std::shared_ptr<MainWindow> m_window;
    std::shared_ptr<OverlayLayout> m_windowLayout;
    std::shared_ptr<OverlayLayout> m_layoutView;
    LayoutScheduler& m_scheduler;
    std::unique_ptr<StatusScreenController> m_statusScreenController;
    std::unique_ptr<MainLayoutController> m_layoutController;

};
