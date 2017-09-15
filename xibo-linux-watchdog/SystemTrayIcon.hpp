#ifndef SYSTEMTRAYICON_HPP
#define SYSTEMTRAYICON_HPP

#include <wx/taskbar.h>
#include <memory>

class MainWindow;
class wxMenuItem;
class PlayerProcessHandler;

class SystemTrayIcon : public wxTaskBarIcon
{
public:

    // TODO: rewrite custom ids
    enum SystemTrayMenu
    {
        START_PLAYER = wxID_HIGHEST + 1,
        CLOSE_PLAYER,
        RESTORE_WATCHDOG,
        HIDE_WATCHDOG,
        CLOSE_WATCHDOG
    };

    SystemTrayIcon(MainWindow* parentWindow,
                   const std::shared_ptr<PlayerProcessHandler>& player);

protected:
    wxMenu* CreatePopupMenu() override;

private:
    void BindMenuToEvents();

private:
    wxMenu* m_trayMenu = nullptr;
    bool m_playerRunning = false;
    bool m_watchdogSHown = true;

    MainWindow* m_parentWindow = nullptr;
    std::shared_ptr<PlayerProcessHandler> m_player;

};

#endif // SYSTEMTRAYICON_HPP
