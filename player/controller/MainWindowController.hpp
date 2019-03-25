#pragma once

#include "view/MainWindow.hpp"
#include "controller/MainLayoutController.hpp"

class LayoutScheduler;

class MainWindowController
{
public:
    MainWindowController(std::shared_ptr<MainWindow> window, LayoutScheduler& scheduler);
    void updateLayout(int layoutId);

private:
    void scaleLayout(const std::shared_ptr<OverlayLayout>& layout);

private:
    std::shared_ptr<MainWindow> m_window;
    LayoutScheduler& m_scheduler;
    std::unique_ptr<MainLayoutController> m_layoutController;

};
