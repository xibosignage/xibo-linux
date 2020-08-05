#include "StatusScreenGtk.hpp"

#include "common/logger/Logging.hpp"

#include <boost/algorithm/string/join.hpp>

StatusScreenGtk::StatusScreenGtk(WindowGtk& parentWindow, int width, int height) :
    WidgetGtk(dialog_),
    dialog_{"", parentWindow.handler()}
{
    WidgetGtk::setSize(width, height);

    firstPage_.add(mainInfoLabel_);
    secondPage_.add(invalidFilesLabel_);
    notebook_.append_page(firstPage_, "Main Info");
    notebook_.append_page(secondPage_, "Invalid Files");

    dialog_.get_content_area()->pack_start(notebook_, Gtk::PACK_EXPAND_WIDGET);
    dialog_.set_title("Status Screen");
    dialog_.add_button("Ok", OkResponseId);
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
    updateTextSize();
    WidgetGtk::setSize(width, height);
}

void StatusScreenGtk::setMainInfo(const std::string& text)
{
    mainInfoLabel_.set_markup(pangoFormat(text));
}

void StatusScreenGtk::setInvalidFiles(const std::vector<std::string>& files)
{
    invalidFilesLabel_.set_markup(pangoFormat(boost::algorithm::join(files, "\n")));
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

void StatusScreenGtk::updateTextSize()
{
    mainInfoLabel_.set_markup(pangoFormat(mainInfoLabel_.get_text()));
    invalidFilesLabel_.set_markup(pangoFormat(invalidFilesLabel_.get_text()));
}
