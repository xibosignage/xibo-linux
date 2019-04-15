#include "MainWindowController.hpp"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>

#include <boost/process/child.hpp>

#include "networking/xmds/XmdsRequestSender.hpp"
#include "config.hpp"
#include "HardwareKey.hpp"

MainWindowController::MainWindowController(Gtk::Window* window, const Glib::RefPtr<Gtk::Builder>& ui) :
    m_ui(ui),
    m_mainWindow(window),
    m_settingsManager(ProjectResources::cmsSettings())
{
    initUi();
    updateControls(m_settingsManager.load());
    connectSignals();
}

void MainWindowController::initUi()
{
    m_ui->get_widget(Resources::Ui::CmsAddressEntry, m_cmsAddress);
    m_ui->get_widget(Resources::Ui::KeyEntry, m_key);
    m_ui->get_widget(Resources::Ui::ResourcesPathEntry, m_resourcesPath);
    m_ui->get_widget(Resources::Ui::BrowseResourcesButton, m_browseResourcesPath);

    m_ui->get_widget(Resources::Ui::UsernameEntry, m_username);
    m_ui->get_widget(Resources::Ui::PasswordEntry, m_password);
    m_ui->get_widget(Resources::Ui::SplashScreenPathEntry, m_splashScreenPath);
    m_ui->get_widget(Resources::Ui::BrowseSplashScreenButton, m_browseSplashScreenPath);
    m_ui->get_widget(Resources::Ui::DomainEntry, m_domain);
    m_ui->get_widget(Resources::Ui::DisplayIdEntry, m_displayId);

    m_ui->get_widget(Resources::Ui::ConnectionStatusLabel, m_connectionStatus);
    m_ui->get_widget(Resources::Ui::SaveButton, m_saveSettings);
    m_ui->get_widget(Resources::Ui::LaunchClientButton, m_launchClient);
    m_ui->get_widget(Resources::Ui::DisplayAdminButton, m_displayAdmin);
    m_ui->get_widget(Resources::Ui::ExitButton, m_exit);
}

void MainWindowController::updateControls(const CmsSettings& settings)
{
    m_cmsAddress->set_text(Glib::ustring{settings.cmsAddress});
    m_key->set_text(Glib::ustring{settings.key});
    m_resourcesPath->set_text(Glib::ustring{settings.resourcesPath});

    m_username->set_text(Glib::ustring{settings.username});
    m_password->set_text(Glib::ustring{settings.password});
    m_domain->set_text(Glib::ustring{settings.domain});
    m_displayId->set_text(Glib::ustring{settings.displayId});
}

void MainWindowController::connectSignals()
{
    m_saveSettings->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onSaveSettingsClicked));
    m_launchClient->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onLaunchClientClicked));
    m_displayAdmin->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onDisplayAdminClicked));
    m_browseResourcesPath->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onBrowseResourcesPathClicked));
    m_exit->signal_clicked().connect(sigc::mem_fun(m_mainWindow, &Gtk::Window::close));
}

void MainWindowController::onSaveSettingsClicked()
{
    auto displayId = getDisplayId();
    auto connectionResult = connectToCms(m_cmsAddress->get_text(), m_key->get_text(), displayId);

    m_connectionStatus->set_text(connectionResult);
    m_displayId->set_text(displayId);

    updateSettings();
}

std::string MainWindowController::getDisplayId()
{
    std::string displayId = m_displayId->get_text();

    return displayId.empty() ? HardwareKey::generate() : displayId;
}

std::string MainWindowController::connectToCms(const std::string& cmsAddress, const std::string& key, const std::string& displayId)
{
    try
    {
        XmdsRequestSender xmdsRequester{cmsAddress, key, displayId};

        auto connectionResult = xmdsRequester.registerDisplay(121, "1.8", "Display").then([](auto future){
            auto [error, result] = future.get();

            if(!error)
                return result.status.message;
            else
                return error.message();
        });

        return connectionResult.get();
    }
    catch(std::exception& e)
    {
        return e.what();
    }
}

void MainWindowController::updateSettings()
{
    CmsSettings settings;

    settings.cmsAddress = m_cmsAddress->get_text();
    settings.key = m_key->get_text();
    settings.resourcesPath = m_resourcesPath->get_text();

    settings.username = m_username->get_text();
    settings.password = m_password->get_text();
    settings.domain = m_domain->get_text();
    settings.displayId = m_displayId->get_text();

    m_settingsManager.update(settings);
}

void MainWindowController::onLaunchClientClicked()
{
    boost::process::child player{ProjectResources::playerBinary()};

    m_mainWindow->close();

    player.wait();
}

void MainWindowController::onBrowseResourcesPathClicked()
{
    Gtk::FileChooserDialog dialog{Resources::ChooseResourcesFolder, Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER};

    dialog.set_transient_for(*m_mainWindow);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();

    switch(result)
    {
        case(Gtk::RESPONSE_OK):
            m_resourcesPath->set_text(dialog.get_filename());
            break;
        default:
            break;
    }
}

void MainWindowController::onDisplayAdminClicked()
{
//    m_mainWindow->show_uri(m_cmsAddress->get_text(), static_cast<unsigned int>(std::time(nullptr)));
}
