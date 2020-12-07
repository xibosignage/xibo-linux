#pragma once

#include "control/widgets/StatusScreen.hpp"
#include "control/widgets/gtk/WindowGtk.hpp"

#include <gtkmm/label.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>

class StatusScreenGtk : public WidgetGtk<Xibo::StatusScreen>
{
    static constexpr const size_t FontScaleFactor = 20;
    static constexpr const int ExitWithoutRestartResponseId = 1;
    static constexpr const int OkResponseId = 0;

public:
    StatusScreenGtk(WindowGtk& parentWindow, int width, int height);

    void setSize(int width, int height) override;
    void setMainInfo(const std::string& text) override;
    void setInvalidFiles(const std::vector<std::string>& files) override;
    ExitWithoutRestartRequested& exitWithoutRestartRequested() override;

    Gtk::Widget& handler() override;

private:
    std::string pangoFormat(const std::string& text);
    void updateTextSize();

private:
    Gtk::Dialog dialog_;
    Gtk::Notebook notebook_;
    Gtk::ScrolledWindow firstPage_;
    Gtk::ScrolledWindow secondPage_;
    Gtk::Label mainInfoLabel_;
    Gtk::Label invalidFilesLabel_;
    ExitWithoutRestartRequested exitWithoutRestartRequested_;
    size_t fontSize_ = 0;
};
