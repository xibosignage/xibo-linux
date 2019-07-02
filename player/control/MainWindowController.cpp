#include "MainWindowController.hpp"
#include "IMainWindow.hpp"

#include "config.hpp"
#include "XlfLayoutFetcher.hpp"

#include "control/common/Image.hpp"
#include "control/layout/IMainLayoutView.hpp"

#include "managers/XiboLayoutScheduler.hpp"

namespace ph = std::placeholders;

const std::string STATUS_SCREEN_KEY = "i";

MainWindowController::MainWindowController(const std::shared_ptr<IMainWindow>& window, XiboLayoutScheduler& scheduler) :
    m_window(window), m_scheduler(scheduler)
{
    m_window->disableWindowDecoration();
    m_window->keyPressed().connect(sigc::mem_fun(this, &MainWindowController::onKeyPressed));
}

void MainWindowController::onKeyPressed(const std::string& pressedKey)
{
    if(pressedKey == STATUS_SCREEN_KEY)
    {
        m_statusScrenRequested.emit();
    }
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


std::unique_ptr<IMainLayout> MainWindowController::createLayout(int layoutId)
{
    auto layout = XlfLayoutFetcher{}.fetch(layoutId);

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

StatusScreenRequested MainWindowController::statusScreenRequested()
{
    return m_statusScrenRequested;
}

std::shared_ptr<IImage> MainWindowController::createSplashScreen()
{
    auto spashImage = std::make_shared<Image>(m_window->width(), m_window->height());

    spashImage->loadFromFile(ProjectResources::splashScreen(), false);

    return spashImage;
}

void MainWindowController::scaleLayout(const std::shared_ptr<IMainLayoutView>& layout)
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
