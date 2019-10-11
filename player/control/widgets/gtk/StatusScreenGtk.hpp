#pragma once

#include "control/widgets/StatusScreen.hpp"
#include "control/widgets/gtk/WindowGtk.hpp"

#include <gtkmm/messagedialog.h>

class StatusScreenGtk : public WidgetGtk<Xibo::StatusScreen>
{
public:
    StatusScreenGtk(WindowGtk& parentWindow, int width, int height);

    void setSize(int width, int height) override;
    void setText(const std::string& text) override;

    Gtk::Widget& get() override;

private:
    std::string pangoFormat(const std::string& text);

private:
    Gtk::MessageDialog dialog_;
    size_t fontSize_ = 0;
};
