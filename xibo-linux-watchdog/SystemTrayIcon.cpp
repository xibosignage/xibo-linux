#include "SystemTrayIcon.hpp"
#include "MainWindow.hpp"
#include "PlayerProcessHandler.hpp"

#include <wx/taskbar.h>
#include <wx/icon.h>
#include <wx/menu.h>

SystemTrayIcon::SystemTrayIcon(MainWindow* parentWindow,
                               const std::shared_ptr<PlayerProcessHandler>& playerApp) :
    m_parentWindow(parentWindow),
    m_playerApp(playerApp)
{
    SetIcon(wxIcon("tray.png"));
}

wxMenu* SystemTrayIcon::CreatePopupMenu()
{
    m_trayMenu = new wxMenu;

    m_closePlayerMenu = new wxMenu;
    m_closePlayerMenu->Append(TERMINATE_PLAYER, "Terminate Player");
    m_closePlayerMenu->Append(KILL_PLAYER, "Kill Player");

    BindClosePlayerMenuToEvents();

    m_trayMenu->Append(START_PLAYER, "Start Player");
    m_trayMenu->Append(CLOSE_PLAYER, "Close Player", m_closePlayerMenu);
    m_trayMenu->Append(RESTORE_WATCHDOG, "Restore Watchdog From Tray");
    m_trayMenu->Append(HIDE_WATCHDOG, "Hide Watchdog To Tray");
    m_trayMenu->Append(CLOSE_WATCHDOG, "Close Watchdog");

    m_trayMenu->Enable(START_PLAYER, !m_playerApp->isRunning());
    m_trayMenu->Enable(CLOSE_PLAYER, m_playerApp->isRunning());

    m_trayMenu->Enable(RESTORE_WATCHDOG, !m_parentWindow->IsShown());
    m_trayMenu->Enable(HIDE_WATCHDOG, m_parentWindow->IsShown());

    BindTrayMenuToEvents();

    return m_trayMenu;
}

void SystemTrayIcon::BindClosePlayerMenuToEvents()
{
    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        if(m_playerApp->isRunning())
        {
            m_playerApp->Stop(SIGTERM);
        }
    }, TERMINATE_PLAYER);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        if(m_playerApp->isRunning())
        {
            m_playerApp->Stop(SIGKILL);
        }
    }, KILL_PLAYER);
}

void SystemTrayIcon::BindTrayMenuToEvents()
{
    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        if(!m_playerApp->isRunning())
        {
            m_playerApp->Run();
        }
    }, START_PLAYER);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_parentWindow->Restore();
    }, RESTORE_WATCHDOG);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_parentWindow->Hide();
    }, HIDE_WATCHDOG);

    Bind(wxEVT_MENU, [=](wxCommandEvent& WXUNUSED(event)){
        m_parentWindow->Close(true);
    }, CLOSE_WATCHDOG);
}
