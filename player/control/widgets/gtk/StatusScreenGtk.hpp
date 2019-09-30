#pragma once

#include "control/widgets/StatusScreen.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/messagedialog.h>

class StatusScreenGtk : public WidgetGtk<Xibo::StatusScreen>
{
public:
    StatusScreenGtk(int width, int height);
    void setText(const std::string& text) override;

    Gtk::Widget& get() override;

private:
    Gtk::MessageDialog dialog_;
};
