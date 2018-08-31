#include "MainWindow.hpp"
#include "constants.hpp"

#include "GtkWindowWrapper.hpp"

MainWindow::MainWindow(int, int, bool, bool, bool, bool)
{
    m_handler = std::make_unique<GtkWindowWrapper>();
    m_handler->set_default_size(640, 480);
//    m_handler->move(x, y);
//    m_handler->set_resizable(resizable);
//    m_handler->set_decorated(decorated);
//    m_handler->set_keep_above(keep_above);
//    if(fullscreen)
//    {
//        m_handler->fullscreen();
//    }
}

void MainWindow::add(std::unique_ptr<IMainLayout> layout)
{
    m_layout = std::move(layout);
    m_handler->add(m_layout->handler());
}

void MainWindow::show()
{
    m_layout->show();
}

IWindowWrapper& MainWindow::handler()
{
    return *m_handler;
}
