#include "StatusScreenGtk.hpp"

const std::string Title = "Status Screen";
const size_t FONT_SCALE_FACTOR = 17;

StatusScreenGtk::StatusScreenGtk(WindowGtk& parentWindow, int width, int height) :
    WidgetGtk(dialog_),
    dialog_{parentWindow.get(), ""}
{
    WidgetGtk::setSize(width, height);

    dialog_.set_title(Title);
    dialog_.signal_response().connect([this](int) { dialog_.hide(); });
}

void StatusScreenGtk::setSize(int width, int height)
{
    fontSize_ = static_cast<size_t>(height) * FONT_SCALE_FACTOR;
    setText(dialog_.property_text().get_value());
    WidgetGtk::setSize(width, height);
}

void StatusScreenGtk::setText(const std::string& text)
{
    dialog_.set_message(pangoFormat(text), true);
}

Gtk::Widget& StatusScreenGtk::get()
{
    return dialog_;
}

std::string StatusScreenGtk::pangoFormat(const std::string& text)
{
    return "<span size=\"" + std::to_string(fontSize_) + "\">" + text + "</span>";
}
