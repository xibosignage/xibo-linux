#include "MainWindowController.hpp"

#include "parsers/MainParser.hpp"
#include "managers/LayoutScheduler.hpp"
#include "view/StatusScreen.hpp"

const int DEFAULT_DIALOG_WIDTH = 640;
const int DEFAULT_DIALOG_HEIGHT = 460;

namespace ph = std::placeholders;

MainWindowController::MainWindowController(std::shared_ptr<MainWindow> window, LayoutScheduler& scheduler) :
    m_window(window), m_scheduler(scheduler)
{
    auto statusScreen = std::make_shared<StatusScreen>(DEFAULT_DIALOG_WIDTH, DEFAULT_DIALOG_HEIGHT);
    m_statusScreenController = std::make_unique<StatusScreenController>(statusScreen);

    m_windowLayout = std::make_shared<OverlayLayout>(window->width(), window->height());
    m_window->addWidget(m_windowLayout);

    m_window->subscribe(EventType::KeyPress,
                        std::bind(&StatusScreenController::onKeyPressed, m_statusScreenController.get(), ph::_1));

//    m_window->disableWindowResize();
//    m_window->disableWindowDecoration();
//    m_window->move(x, y);
//    m_window->fullscreen();
}

void MainWindowController::updateLayout(int layoutId)
{
    auto layoutModel = MainParser{}.parseLayoutModel(layoutId);
    auto layoutView = std::make_shared<OverlayLayout>(layoutModel->width(), layoutModel->height());
    m_layoutController = std::make_unique<MainLayoutController>(layoutModel, layoutView);

    m_layoutController->subscribe(EventType::DurationExpired, [this](const Event&){
        updateLayout(m_scheduler.nextLayoutId());
    });

    scaleLayout(layoutView);

    m_windowLayout->removeMainWidget();
    m_windowLayout->addMainWidget(layoutView);
    layoutView->showAll();
}

void MainWindowController::scaleLayout(const std::shared_ptr<OverlayLayout>& layout)
{
    double scaleX = static_cast<double>(m_window->width()) / layout->width();
    double scaleY = static_cast<double>(m_window->height()) / layout->height();
    double scaleFactor = std::min(scaleX, scaleY);

    layout->scale(scaleFactor, scaleFactor);
}

