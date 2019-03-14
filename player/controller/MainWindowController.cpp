#include "MainWindowController.hpp"

#include "parsers/MainParser.hpp"
#include "managers/LayoutScheduler.hpp"

MainWindowController::MainWindowController(std::shared_ptr<MainWindow> window, LayoutScheduler& scheduler) :
    m_window(window), m_scheduler(scheduler)
{
//    m_window->disableWindowResize();
//    m_window->disableWindowDecoration();
//    m_window->move(x, y);
//    m_window->fullscreen();
}

void MainWindowController::updateLayout(int layoutId)
{
    MainParser parser;
    auto layoutModel = parser.parseLayoutModel(layoutId);
    auto layoutView = std::make_shared<OverlayLayout>(layoutModel->width(), layoutModel->height());
    m_layoutController = std::make_unique<MainLayoutController>(layoutModel, layoutView);

    m_layoutController->subscribe(EventType::DurationExpired, [this](const Event&){
        updateLayout(m_scheduler.nextLayoutId());
    });

    scaleLayout(layoutView);

    m_window->removeWidget();
    m_window->addWidget(layoutView);
    layoutView->showAll();
}

void MainWindowController::scaleLayout(const std::shared_ptr<OverlayLayout>& layout)
{
    double scaleX = static_cast<double>(m_window->width()) / layout->width();
    double scaleY = static_cast<double>(m_window->height()) / layout->height();
    double scaleFactor = std::min(scaleX, scaleY);

    layout->scale(scaleFactor, scaleFactor);
}

