#include "SystemTrayIcon.hpp"
#include "MainWindow.hpp"
#include "PlayerProcessHandler.hpp"

#include <wx/taskbar.h>
#include <wx/icon.h>
#include <wx/menu.h>

SystemTrayIcon::SystemTrayIcon(MainWindow* parentWindow,
                               const std::shared_ptr<PlayerProcessHandler>& player) :
    m_parentWindow(parentWindow),
    m_player(player)
{
    SetIcon(wxIcon("tray.png"));
}

wxMenu* SystemTrayIcon::CreatePopupMenu()
{
    m_trayMenu = new wxMenu;

    m_trayMenu->Append(START_PLAYER, "Start Player");
    m_trayMenu->Append(CLOSE_PLAYER, "Close Player");
    m_trayMenu->Append(RESTORE_WATCHDOG, "Restore Watchdog From Tray");
    m_trayMenu->Append(HIDE_WATCHDOG, "Hide Watchdog To Tray");
    m_trayMenu->Append(CLOSE_WATCHDOG, "Close Watchdog");

    m_trayMenu->Enable(START_PLAYER, !m_playerRunning);
    m_trayMenu->Enable(CLOSE_PLAYER, m_playerRunning);

    m_trayMenu->Enable(RESTORE_WATCHDOG, !m_watchdogSHown);
    m_trayMenu->Enable(HIDE_WATCHDOG, m_watchdogSHown);

    BindMenuToEvents();

    return m_trayMenu;
}

void SystemTrayIcon::BindMenuToEvents()
{
    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_playerRunning = true;
        m_player->Run();
    }, START_PLAYER);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_playerRunning = false;
        m_player->Stop();
    }, CLOSE_PLAYER);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_watchdogSHown = true;
        m_parentWindow->Restore();
    }, RESTORE_WATCHDOG);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_watchdogSHown = false;
        m_parentWindow->Hide();
    }, HIDE_WATCHDOG);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_parentWindow->Close(true);
    }, CLOSE_WATCHDOG);
}
