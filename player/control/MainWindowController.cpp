#include "MainWindowController.hpp"

#include "StatusScreen.hpp"

#include "control/layout/MainLayoutView.hpp"
#include "control/common/MainCompositor.hpp"
#include "control/common/Image.hpp"
#include "config.hpp"
#include "managers/XiboLayoutScheduler.hpp"

const int DEFAULT_DIALOG_WIDTH = 640;
const int DEFAULT_DIALOG_HEIGHT = 460;

namespace ph = std::placeholders;

MainWindowController::MainWindowController(std::shared_ptr<MainWindow> window, XiboLayoutScheduler& scheduler) :
    m_window(window), m_scheduler(scheduler)
{
    auto statusScreen = std::make_shared<StatusScreen>(DEFAULT_DIALOG_WIDTH, DEFAULT_DIALOG_HEIGHT);
    m_statusScreenController = std::make_unique<StatusScreenController>(scheduler, statusScreen);

    m_window->keyPressed().connect(std::bind(&StatusScreenController::onKeyPressed, m_statusScreenController.get(), ph::_1));
    m_window->disableWindowDecoration();
}

void MainWindowController::updateLayout(int layoutId)
{
    m_layout.reset();

    if(layoutId != EMPTY_LAYOUT_ID)
    {
        showLayout(layoutId);
    }
    else
    {
        showSplashScreen();
    }
}

void MainWindowController::showLayout(int layoutId)
{
    m_layout = createLayout(layoutId);

    m_window->setWidget(m_layout->view());
    m_window->showAll();
}

std::unique_ptr<MainLayout> MainWindowController::createLayout(int layoutId)
{
    MainCompositor parser;
    auto layout = parser.parseLayout(layoutId);

    layout->expired().connect([this](){
        updateLayout(m_scheduler.nextLayoutId());
    });

    scaleLayout(layout->view());

    return layout;
}

void MainWindowController::showSplashScreen()
{
    m_window->setWidget(createSplashScreen());
    m_window->showAll();
}

std::shared_ptr<Widget> MainWindowController::createSplashScreen()
{
    auto spashImage = std::make_shared<Image>(m_window->width(), m_window->height());

    spashImage->loadFromFile(ProjectResources::splashScreen(), false);

    return spashImage;
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
