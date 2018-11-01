#include "MainWindow.hpp"
#include "constants.hpp"

#include "adaptors/IWindowAdaptor.hpp"
#include "adaptors/IOverlayAdaptor.hpp"

#include <cassert>

MainWindow::MainWindow(std::unique_ptr<IWindowAdaptor>&& handler) :
    m_handler(std::move(handler))
{
    m_handler->disableWindowDecoration();
    m_handler->disableWindowResize();

    m_handler->connectToHandlerResize([=](){
        if(m_layout)
        {
            scaleLayout(*m_layout);
        }
    });
}

void MainWindow::setSize(int width, int height)
{
    checkWindowNewSize(width, height);

    m_handler->setSize(width, height);
}

// TODO specify after adding Monitor max width and height
void MainWindow::checkWindowNewSize(int width, int height)
{
    if(width < MIN_DISPLAY_WIDTH || height < MIN_DISPLAY_HEIGHT)
        throw std::invalid_argument("Width or height is too small/large");
}

void MainWindow::setPos(int x, int y)
{
    checkWindowCoordinates(x, y);

    m_handler->move(x, y);
}

// TODO specify after adding Monitor max width and height
void MainWindow::checkWindowCoordinates(int x, int y)
{
    if(x < MIN_XPOS || y < MIN_YPOS)
        throw std::invalid_argument("x or y position should be greater than 0");
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
    return m_handler->isShown();
}

void MainWindow::addLayout(std::unique_ptr<IMainLayout>&& layout)
{
    assert(layout);

    m_handler->add(layout->handler());
    scaleLayout(*layout);
    m_layout = std::move(layout);
}

void MainWindow::scaleLayout(IMainLayout& layout)
{
    double scaleX = static_cast<double>(m_handler->width()) / layout.width();
    double scaleY = static_cast<double>(m_handler->height()) / layout.height();
    double scaleFactor = std::min(scaleX, scaleY);
    layout.scale(scaleFactor, scaleFactor);
}

void MainWindow::showLayout()
{
    assert(m_layout);

    m_layout->show();
}

IWindowAdaptor& MainWindow::handler()
{
    return *m_handler;
}
