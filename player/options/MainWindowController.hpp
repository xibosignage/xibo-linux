#pragma once

#include <gtkmm/builder.h>
#include <gtkmm/window.h>

#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include "Resources.hpp"
#include "config/CmsSettings.hpp"
#include "config/PlayerSettings.hpp"

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
    void onBrowseResourcesPathClicked();
    std::string createDefaultResourceDir();

private:
    Glib::RefPtr<Gtk::Builder> ui_;
    Gtk::Window* mainWindow_;
    CmsSettings cmsSettings_;
    PlayerSettings playerSettings_;

    Gtk::Entry* cmsAddressField_;
    Gtk::Entry* keyField_;
    Gtk::Entry* resourcesPathField_;
    Gtk::Button* browseResourcesPath_;

    Gtk::Entry* usernameField_;
    Gtk::Entry* passwordField_;
    Gtk::Entry* splashScreenPath_;
    Gtk::Button* browseSplashScreenPath_;
    Gtk::Entry* domainField_;
    Gtk::Entry* displayIdField_;

    Gtk::Label* connectionStatus_;
    Gtk::Button* saveSettings_;
    Gtk::Button* exit_;
};
