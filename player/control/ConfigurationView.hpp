#pragma once

#include "control/common/Widget.hpp"

#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>

class ConfigurationView : public Widget
{
public:
    ConfigurationView(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    void showAll() override;
    Gtk::Widget& get() override;

    std::string host() const;
    std::string serverKey() const;
    std::string hardwareKey() const;

private:
    Gtk::Box m_mainVBox;
    Gtk::Box m_hostHBox, m_serverKeyBox, m_hardwareKeyBox;
    Gtk::Label m_hostLabel, m_serverKeyLabel, m_hardwareKeyLabel;
    Gtk::Entry m_hostEntry, m_serverKeyEntry, m_hardwareKeyEntry;
    Gtk::Button m_confirmButton;

};
