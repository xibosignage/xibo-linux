#include "StatusScreenGtk.hpp"

const std::string Title = "Status Screen";

StatusScreenGtk::StatusScreenGtk(int width, int height) : WidgetGtk(dialog_), dialog_{""}
{
    WidgetGtk::setSize(width, height);

    dialog_.set_title(Title);
    dialog_.signal_response().connect([this](int) { dialog_.hide(); });
}

void StatusScreenGtk::setText(const std::string& text)
{
    dialog_.set_message(text);
}

Gtk::Widget& StatusScreenGtk::get()
{
    return dialog_;
}
