#pragma once

#include "control/widgets/StatusScreen.hpp"
#include "control/widgets/gtk/WindowGtk.hpp"

#include <gtkmm/messagedialog.h>

class StatusScreenGtk : public WidgetGtk<Xibo::StatusScreen>
{
    static constexpr const size_t FontScaleFactor = 17;
    static constexpr const int ExitWithoutRestartResponseId = 1;

public:
    StatusScreenGtk(WindowGtk& parentWindow, int width, int height);

    void setSize(int width, int height) override;
    void setText(const std::string& text) override;
    ExitWithoutRestartRequested& exitWithoutRestartRequested() override;

    Gtk::Widget& handler() override;

private:
    std::string pangoFormat(const std::string& text);

private:
    Gtk::MessageDialog dialog_;
    ExitWithoutRestartRequested exitWithoutRestartRequested_;
    size_t fontSize_ = 0;
};
