#include "StatusScreenGtk.hpp"

#include "common/logger/Logging.hpp"

StatusScreenGtk::StatusScreenGtk(WindowGtk& parentWindow, int width, int height) :
    WidgetGtk(dialog_),
    dialog_{parentWindow.handler(), ""}
{
    WidgetGtk::setSize(width, height);

    dialog_.set_title("Status Screen");
    dialog_.add_button("Exit and don't restart", ExitWithoutRestartResponseId);
    dialog_.signal_response().connect([this](int responseId) {
        dialog_.hide();
        if (responseId == ExitWithoutRestartResponseId)
        {
            exitWithoutRestartRequested_();
        }
    });
}

void StatusScreenGtk::setSize(int width, int height)
{
    fontSize_ = static_cast<size_t>(height) * FontScaleFactor;
    setText(dialog_.property_text().get_value());
    WidgetGtk::setSize(width, height);
}

void StatusScreenGtk::setText(const std::string& text)
{
    dialog_.set_message(pangoFormat(text), true);
}

ExitWithoutRestartRequested& StatusScreenGtk::exitWithoutRestartRequested()
{
    return exitWithoutRestartRequested_;
}

Gtk::Widget& StatusScreenGtk::handler()
{
    return dialog_;
}

std::string StatusScreenGtk::pangoFormat(const std::string& text)
{
    return "<span size=\"" + std::to_string(fontSize_) + "\">" + text + "</span>";
}
