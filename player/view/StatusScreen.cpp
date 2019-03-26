#include "StatusScreen.hpp"

#include "utils/logger/Logging.hpp"

const int DEFAULT_BUTTON_WIDTH = 100;
const int DEFAULT_BUTTON_HEIGHT = 50;

StatusScreen::StatusScreen(int width, int height) :
    Widget(m_configButton), m_dialog{""}
{
	m_dialog.set_title("Status Screen");
    m_dialog.set_size_request(width, height);

    m_configButton.set_label("STATUS");
    m_configButton.set_size_request(DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT);
    m_configButton.signal_clicked().connect([this]{
        m_dialog.set_message(R"(
        General Info:
        Date - yyyy-mm-dd hh:mm:ss
        Version - 1.8
        Code Version - 108
        Content Management System: https://linuxplayer.xibo.co.uk/xmds.php
        Storage Selected: path, percantage occupied
        Display Name: name
        Registered: true/false
        Current Layout: layoutid
        Screen Size: window size
        Memory Limit: mb
        Memory Allocation: mb
        Image Cache Hit Rate:
        Screen Shot Request Interval
        Number of Stats ready to send: 0 - yyyy-mm-dd hh:mm:ss
        Number of Logs ready to send: 0 - yyyy-mm-dd hh:mm:ss

        CMS Status
        Register: registered. Check at yyyy-mm-dd hh:mm:ss
        Schedule: up to date
        Required Files: up to date 56/56
        Queued Network Connections - F: 0 / N: 0. HTTP: 0 / Idle: 0
        Number of Concurrent Failed XMDS Connections: 0

        Schedule Status
        All layouts (*= not scheduled): 49*(D),
        Scheduled Layout: 49,
        Valid Layouts: 49,
        Invalid Layout:
        Next Schedule Update: yyyy-mm-dd hh:mm:ss)");
        showDialog();
    });
}

void StatusScreen::setText(const std::string& text)
{
    m_dialog.set_message(text);
}

void StatusScreen::showDialog()
{
    m_dialog.run();
    m_dialog.close();
}

Gtk::Widget& StatusScreen::get()
{
	return m_configButton;
}
