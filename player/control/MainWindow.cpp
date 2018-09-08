#include "MainWindow.hpp"
#include "constants.hpp"

#include "adaptors/GtkWindowAdaptor.hpp"
#include "adaptors/IOverlayAdaptor.hpp"

MainWindow::MainWindow(int width, int height) :
    MainWindow(width, height, std::make_unique<GtkWindowAdaptor>())
{
}

MainWindow::MainWindow(int width, int height, std::unique_ptr<IWindowAdaptor> handler) :
    m_handler(std::move(handler))
{
    if(width < MIN_DISPLAY_WIDTH || width > MAX_DISPLAY_WIDTH || height < MIN_DISPLAY_HEIGHT || height > MAX_DISPLAY_HEIGHT)
        throw std::runtime_error("Width or height is too small/large");

    m_handler->setDefaultSize(width, height);
    m_handler->disableWindowDecoration();
    m_handler->disableWindowResize();
}

// FIXME add check for screen size and tests for this
void MainWindow::setPos(int x, int y)
{
    if(x < 0 || y < 0)
        throw std::runtime_error("x or y position should be greater than 0");

    m_handler->move(x, y);
}

void MainWindow::setKeepAbove(bool keepAbove)
{
    m_handler->setKeepAbove(keepAbove);
}

void MainWindow::setFullscreen(bool fullscreen)
{
    if(fullscreen)
        m_handler->fullscreen();
    else
        m_handler->unfullscreen();
}

void MainWindow::setCursorVisible(bool cursorVisible)
{
    m_handler->setCursorVisible(cursorVisible);
}

bool MainWindow::isVisible() const
{
    return m_handler->isVisible();
}

void MainWindow::addLayout(std::unique_ptr<IMainLayout> layout)
{
    if(!layout)
        throw std::runtime_error("Invalid layout given");

    m_layout = std::move(layout);
    m_handler->add(m_layout->handler());
}

void MainWindow::showLayout()
{
    if(m_layout)
    {
        m_layout->show();
    }
}

IWindowAdaptor& MainWindow::handler()
{
    return *m_handler;
}
