#pragma once

#include "MainWindow.hpp"
#include "StatusScreenController.hpp"
#include "control/layout/MainLayout.hpp"

class LayoutScheduler;
class MainLayoutView;

class MainWindowController
{
public:
    MainWindowController(std::shared_ptr<MainWindow> window, LayoutScheduler& scheduler);
    void updateLayout(int layoutId);

private:
    void scaleLayout(const std::shared_ptr<MainLayoutView>& layout);

private:
    std::shared_ptr<MainWindow> m_window;
    LayoutScheduler& m_scheduler;
    std::unique_ptr<StatusScreenController> m_statusScreenController;
    std::unique_ptr<MainLayout> m_layout;

};
