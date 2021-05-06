#include "MainWindowController.hpp"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>

#include "cms/xmds/XmdsRequestSender.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"
#include "common/system/System.hpp"
#include "config/AppConfig.hpp"

MainWindowController::MainWindowController(Gtk::Window* window, const Glib::RefPtr<Gtk::Builder>& ui) :
    ui_(ui),
    mainWindow_(window)
{
    cmsSettings_.fromFile(AppConfig::cmsSettingsPath());
    playerSettings_.fromFile(AppConfig::playerSettingsPath());

    initUi();
    updateControls(cmsSettings_);
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
    ui_->get_widget(Resources::Ui::ExitButton, exit_);
}

void MainWindowController::updateControls(const CmsSettings& settings)
{
    cmsAddressField_->set_text(Glib::ustring{settings.address()});
    keyField_->set_text(Glib::ustring{settings.key()});
    FilePath path = settings.resourcesPath();
    resourcesPathField_->set_text(Glib::ustring{path.string()});
    usernameField_->set_text(Glib::ustring{settings.username()});
    passwordField_->set_text(Glib::ustring{settings.password()});
    domainField_->set_text(Glib::ustring{settings.domain()});
    displayIdField_->set_text(Glib::ustring{settings.displayId()});
}

void MainWindowController::connectSignals()
{
    saveSettings_->signal_clicked().connect(std::bind(&MainWindowController::onSaveSettingsClicked, this));
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
    auto keyHash = static_cast<Md5Hash>(System::hardwareKey());

    return displayId.empty() ? static_cast<std::string>(keyHash) : displayId;
}

std::string MainWindowController::connectToCms(const std::string& cmsAddress,
                                               const std::string& key,
                                               const std::string& displayId)
{
    try
    {
        XmdsRequestSender xmdsRequester{cmsAddress, key, displayId};

                
        auto connectionResult =
            xmdsRequester.registerDisplay(AppConfig::codeVersion(), AppConfig::releaseVersion(), playerSettings_.displayName())
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
    cmsSettings_.address().setValue(cmsAddressField_->get_text());
    cmsSettings_.key().setValue(keyField_->get_text());
    std::string path = resourcesPathField_->get_text();
    cmsSettings_.resourcesPath().setValue(path.empty() ? createDefaultResourceDir() : path);
    cmsSettings_.displayId().setValue(displayIdField_->get_text());

    cmsSettings_.updateProxy(domainField_->get_text(), usernameField_->get_text(), passwordField_->get_text());

    cmsSettings_.saveTo(AppConfig::cmsSettingsPath());
}

std::string MainWindowController::createDefaultResourceDir()
{
    auto defaultDir = AppConfig::configDirectory() / "resources";
    if (!FileSystem::exists(defaultDir))
    {
        bool result = FileSystem::createDirectory(defaultDir);
        if (!result)
        {
            Log::error("Unable to create resources directory");
        }
    }
    return defaultDir.string();
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
