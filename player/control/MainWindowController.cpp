#include "MainWindowController.hpp"

#include "StatusScreen.hpp"

#include "control/layout/MainLayoutView.hpp"
#include "control/common/MainCompositor.hpp"
#include "managers/LayoutScheduler.hpp"

const int DEFAULT_DIALOG_WIDTH = 640;
const int DEFAULT_DIALOG_HEIGHT = 460;

namespace ph = std::placeholders;

MainWindowController::MainWindowController(std::shared_ptr<MainWindow> window, LayoutScheduler& scheduler) :
    m_window(window), m_scheduler(scheduler)
{
    auto statusScreen = std::make_shared<StatusScreen>(DEFAULT_DIALOG_WIDTH, DEFAULT_DIALOG_HEIGHT);
    m_statusScreenController = std::make_unique<StatusScreenController>(statusScreen);

    m_window->keyPressed().connect(std::bind(&StatusScreenController::onKeyPressed, m_statusScreenController.get(), ph::_1));

    m_window->disableWindowResize();
    m_window->disableWindowDecoration();
}

void MainWindowController::updateLayout(int layoutId)
{
    MainCompositor parser;
    auto [layout, layoutView] = parser.parseLayout(layoutId);

    m_layout = std::move(layout);
    m_layout->expired().connect([this](){
        updateLayout(m_scheduler.nextLayoutId());
    });

    scaleLayout(layoutView);

    m_window->removeWidget();
    m_window->addWidget(layoutView);
    layoutView->showAll();
}

void MainWindowController::scaleLayout(const std::shared_ptr<MainLayoutView>& layout)
{
    double scaleX = static_cast<double>(m_window->width()) / layout->width();
    double scaleY = static_cast<double>(m_window->height()) / layout->height();
    double scaleFactor = std::min(scaleX, scaleY);

    layout->scale(scaleFactor, scaleFactor);
}

void MainWindowController::updateWindowDimensions(const PlayerSettings::Dimensions& dimensions)
{
    if(shouldBeFullscreen(dimensions))
    {
        m_window->fullscreen();
    }
    else
    {
        setWindowSize(dimensions.width, dimensions.height);
        setWindowPos(dimensions.x, dimensions.y);
    }
}

void MainWindowController::setWindowSize(int width, int height)
{
    if(width != m_window->width() || height != m_window->height())
    {
        m_window->setSize(width, height);
    }
}

void MainWindowController::setWindowPos(int x, int y)
{
    if(x != m_window->x() || y != m_window->y())
    {
        m_window->move(x, y);
    }
}

bool MainWindowController::shouldBeFullscreen(const PlayerSettings::Dimensions& dimensions) const
{
    return dimensions.width == 0 && dimensions.height == 0;
}
