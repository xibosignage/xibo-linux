#pragma once

#include <gtkmm/window.h>

class MainLayout;

class MainWindow : public Gtk::Window
{
public:
    MainWindow(int x, int y, bool resizable, bool decorated, bool fullscreen, bool keep_above);
    ~MainWindow() = default;

    MainWindow(const MainWindow& other) = delete;
    MainWindow& operator=(const MainWindow& other) = delete;

    void add(MainLayout& layout);

private:
    void on_window_realize();

};
