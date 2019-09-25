#pragma once

#include <gtkmm/builder.h>
#include <gtkmm/window.h>

#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include "Resources.hpp"
#include "common/settings/CmsSettings.hpp"

class MainWindowController
{
public:
    MainWindowController(Gtk::Window* window, const Glib::RefPtr<Gtk::Builder>& ui);

private:
    void initUi();
    void updateControls(const CmsSettings& settings);
    void connectSignals();

    void onSaveSettingsClicked();
    std::string getDisplayId();
    std::string connectToCms(const std::string& cmsAddress, const std::string& key, const std::string& displayId);
    void updateSettings();
    void showSettingsSavedMessage();
    void onLaunchClientClicked();
    void onBrowseResourcesPathClicked();
    void onDisplayAdminClicked();

private:
    Glib::RefPtr<Gtk::Builder> m_ui;
    Gtk::Window* m_mainWindow;
    CmsSettings m_settings;

    Gtk::Entry* m_cmsAddressField;
    Gtk::Entry* m_keyField;
    Gtk::Entry* m_resourcesPathField;
    Gtk::Button* m_browseResourcesPath;

    Gtk::Entry* m_usernameField;
    Gtk::Entry* m_passwordField;
    Gtk::Entry* m_splashScreenPath;
    Gtk::Button* m_browseSplashScreenPath;
    Gtk::Entry* m_domainField;
    Gtk::Entry* m_displayIdField;

    Gtk::Label* m_connectionStatus;
    Gtk::Button* m_saveSettings;
    Gtk::Button* m_launchClient;
    Gtk::Button* m_displayAdmin;
    Gtk::Button* m_exit;
};
