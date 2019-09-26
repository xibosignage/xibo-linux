#include "StatusScreen.hpp"

StatusScreen::StatusScreen(int width, int height) : Widget(dialog_), dialog_{""}
{
    setSize(width, height);

    dialog_.set_title("Status Screen");
    dialog_.signal_response().connect([this](int) { dialog_.hide(); });
}

void StatusScreen::setText(const std::string& text)
{
    dialog_.set_message(text);
}

Gtk::Widget& StatusScreen::get()
{
    return dialog_;
}
