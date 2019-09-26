#include "MainWindowController.hpp"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>

#include <boost/process/child.hpp>

#include "HardwareKey.hpp"
#include "config.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"

MainWindowController::MainWindowController(Gtk::Window* window, const Glib::RefPtr<Gtk::Builder>& ui) :
    ui_(ui),
    mainWindow_(window)
{
    settings_.loadFrom(ProjectResources::cmsSettingsPath());

    initUi();
    updateControls(settings_);
    connectSignals();
}

void MainWindowController::initUi()
{
    ui_->get_widget(Resources::Ui::CmsAddressEntry, cmsAddressField_);
    ui_->get_widget(Resources::Ui::KeyEntry, keyField_);
    ui_->get_widget(Resources::Ui::ResourcesPathEntry, resourcesPathField_);
    ui_->get_widget(Resources::Ui::BrowseResourcesButton, browseResourcesPath_);

    ui_->get_widget(Resources::Ui::UsernameEntry, usernameField_);
    ui_->get_widget(Resources::Ui::PasswordEntry, passwordField_);
    ui_->get_widget(Resources::Ui::SplashScreenPathEntry, splashScreenPath_);
    ui_->get_widget(Resources::Ui::BrowseSplashScreenButton, browseSplashScreenPath_);
    ui_->get_widget(Resources::Ui::DomainEntry, domainField_);
    ui_->get_widget(Resources::Ui::DisplayIdEntry, displayIdField_);

    ui_->get_widget(Resources::Ui::ConnectionStatusLabel, connectionStatus_);
    ui_->get_widget(Resources::Ui::SaveButton, saveSettings_);
    ui_->get_widget(Resources::Ui::LaunchClientButton, launchClient_);
    ui_->get_widget(Resources::Ui::DisplayAdminButton, displayAdmin_);
    ui_->get_widget(Resources::Ui::ExitButton, exit_);
}

void MainWindowController::updateControls(const CmsSettings& settings)
{
    cmsAddressField_->set_text(Glib::ustring{settings.cmsAddress});
    keyField_->set_text(Glib::ustring{settings.key});
    resourcesPathField_->set_text(Glib::ustring{settings.resourcesPath});

    usernameField_->set_text(Glib::ustring{settings.username});
    passwordField_->set_text(Glib::ustring{settings.password});
    domainField_->set_text(Glib::ustring{settings.domain});
    displayIdField_->set_text(Glib::ustring{settings.displayId});
}

void MainWindowController::connectSignals()
{
    saveSettings_->signal_clicked().connect(std::bind(&MainWindowController::onSaveSettingsClicked, this));
    launchClient_->signal_clicked().connect(std::bind(&MainWindowController::onLaunchClientClicked, this));
    displayAdmin_->signal_clicked().connect(std::bind(&MainWindowController::onDisplayAdminClicked, this));
    browseResourcesPath_->signal_clicked().connect(
        std::bind(&MainWindowController::onBrowseResourcesPathClicked, this));
    exit_->signal_clicked().connect(std::bind(&Gtk::Window::close, mainWindow_));
}

void MainWindowController::onSaveSettingsClicked()
{
    auto displayId = getDisplayId();
    auto connectionResult = connectToCms(cmsAddressField_->get_text(), keyField_->get_text(), displayId);

    connectionStatus_->set_text(connectionResult);
    displayIdField_->set_text(displayId);

    updateSettings();
}

std::string MainWindowController::getDisplayId()
{
    std::string displayId = displayIdField_->get_text();

    return displayId.empty() ? HardwareKey::generate() : displayId;
}

std::string MainWindowController::connectToCms(const std::string& cmsAddress, const std::string& key,
                                               const std::string& displayId)
{
    try
    {
        XmdsRequestSender xmdsRequester{cmsAddress, key, displayId};

        auto connectionResult =
            xmdsRequester.registerDisplay(ProjectResources::codeVersion(), ProjectResources::version(), "Display")
                .then([](auto future) {
                    auto [error, result] = future.get();

                    if (!error)
                        return result.status.message;
                    else
                        return error.message();
                });

        return connectionResult.get();
    }
    catch (std::exception& e)
    {
        return e.what();
    }
}

void MainWindowController::updateSettings()
{
    settings_.cmsAddress = cmsAddressField_->get_text();
    settings_.key = keyField_->get_text();
    std::string path = resourcesPathField_->get_text();
    settings_.resourcesPath = path.empty() ? ProjectResources::defaultResourcesDir().string() : path;

    settings_.username = usernameField_->get_text();
    settings_.password = passwordField_->get_text();
    settings_.domain = domainField_->get_text();
    settings_.displayId = displayIdField_->get_text();

    settings_.saveTo(ProjectResources::cmsSettingsPath());
}

void MainWindowController::onLaunchClientClicked()
{
    boost::process::child player{ProjectResources::playerBinary()};

    mainWindow_->close();

    player.detach();
}

void MainWindowController::onBrowseResourcesPathClicked()
{
    Gtk::FileChooserDialog dialog{Resources::ChooseResourcesFolder, Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER};

    dialog.set_transient_for(*mainWindow_);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();

    switch (result)
    {
        case (Gtk::RESPONSE_OK): resourcesPathField_->set_text(dialog.get_filename()); break;
        default: break;
    }
}

void MainWindowController::onDisplayAdminClicked()
{
    //    m_mainWindow->show_uri(m_cmsAddress->get_text(), static_cast<unsigned int>(std::time(nullptr)));
}
