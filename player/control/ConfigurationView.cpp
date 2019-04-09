#include "ConfigurationView.hpp"

ConfigurationView::ConfigurationView(const std::string& host, const std::string& serverKey, const std::string& hardwareKey) :
    Widget(m_mainVBox),
    m_mainVBox(Gtk::ORIENTATION_VERTICAL)
{
    m_hostLabel.set_text("Host");
    m_serverKeyLabel.set_text("Server Key");
    m_hardwareKeyLabel.set_text("Hardware Key");
    m_confirmButton.set_label("Start Player");

    m_hostEntry.set_text(host);
    m_serverKeyEntry.set_text(serverKey);
    m_hardwareKeyEntry.set_text(hardwareKey);

    m_hostHBox.pack_start(m_hostLabel);
    m_hostHBox.pack_start(m_hostEntry);

    m_serverKeyBox.pack_start(m_serverKeyLabel);
    m_serverKeyBox.pack_start(m_serverKeyEntry);

    m_hardwareKeyBox.pack_start(m_hardwareKeyLabel);
    m_hardwareKeyBox.pack_start(m_hardwareKeyEntry);

    m_mainVBox.pack_start(m_hostHBox, Gtk::PACK_EXPAND_PADDING);
    m_mainVBox.pack_start(m_serverKeyBox, Gtk::PACK_EXPAND_PADDING);
    m_mainVBox.pack_start(m_hardwareKeyBox, Gtk::PACK_EXPAND_PADDING);
    m_mainVBox.pack_start(m_confirmButton, Gtk::PACK_EXPAND_PADDING);

    m_mainVBox.set_margin_left(10);
    m_mainVBox.set_margin_right(10);
    m_mainVBox.set_margin_top(10);
    m_mainVBox.set_margin_bottom(10);

    m_confirmButton.signal_clicked().connect([](){
        // FIXME
    });
}

void ConfigurationView::showAll()
{
    m_mainVBox.show_all();
}

Gtk::Widget& ConfigurationView::get()
{
    return m_mainVBox;
}

std::string ConfigurationView::host() const
{
    return m_hostEntry.get_text();
}

std::string ConfigurationView::serverKey() const
{
    return m_serverKeyEntry.get_text();
}

std::string ConfigurationView::hardwareKey() const
{
    return m_hardwareKeyEntry.get_text();
}
