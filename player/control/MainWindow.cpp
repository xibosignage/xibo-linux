#include "MainWindow.hpp"
#include "MainLayout.hpp"
#include "Monitor.hpp"

#include "constants.hpp"

MainWindow::MainWindow(int x, int y, bool resizable, bool decorated, bool fullscreen, bool keep_above)
{
    set_default_size(640, 480);
//    move(x, y);
//    set_resizable(resizable);
//    set_decorated(decorated);
//    if(fullscreen)
//    {
//        fullscreen();
//    }
//    if(keep_above)
//    {
//        set_keep_above();
//    }
    signal_realize().connect(sigc::mem_fun(*this, &MainWindow::on_window_realize));
}

void MainWindow::add(MainLayout& layout)
{
    std::shared_ptr<IMonitor> monitor = std::make_shared<Monitor>(*this);
    layout.scale_to_monitor_size(monitor);
    Gtk::Window::add(layout);
}

// FIXME: add unit test
void MainWindow::on_window_realize()
{
    Gtk::Window::on_realize();

//    auto window = get_window();
//    auto cursor = Gdk::Cursor::create(Gdk::BLANK_CURSOR);
//    window->set_cursor(cursor);
}
