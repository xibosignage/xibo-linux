#include "StatusScreen.hpp"

StatusScreen::StatusScreen(int width, int height) : Widget(m_dialog), m_dialog{""}
{
    setSize(width, height);

    m_dialog.set_title("Status Screen");
    m_dialog.signal_response().connect([this](int) { m_dialog.hide(); });
}

void StatusScreen::setText(const std::string& text)
{
    m_dialog.set_message(text);
}

Gtk::Widget& StatusScreen::get()
{
    return m_dialog;
}
