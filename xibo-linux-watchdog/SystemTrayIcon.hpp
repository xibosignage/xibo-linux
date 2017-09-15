#ifndef SYSTEMTRAYICON_HPP
#define SYSTEMTRAYICON_HPP

#include <wx/taskbar.h>
#include <memory>

class MainWindow;
class PlayerProcessHandler;
class wxMenuItem;

class SystemTrayIcon : public wxTaskBarIcon
{
public:
    SystemTrayIcon(MainWindow* parentWindow,
                   const std::shared_ptr<PlayerProcessHandler>& playerApp);

protected:
    wxMenu* CreatePopupMenu() override;

private:
    void BindTrayMenuToEvents();
    void BindClosePlayerMenuToEvents();

private:
    enum SystemTrayMenu
    {
        START_PLAYER = wxID_HIGHEST + 1,
        CLOSE_PLAYER,
        TERMINATE_PLAYER,
        KILL_PLAYER,
        RESTORE_WATCHDOG,
        HIDE_WATCHDOG,
        CLOSE_WATCHDOG
    };

    wxMenu* m_trayMenu = nullptr;
    wxMenu* m_closePlayerMenu = nullptr;

    MainWindow* m_parentWindow = nullptr;
    std::shared_ptr<PlayerProcessHandler> m_playerApp;

};

#endif // SYSTEMTRAYICON_HPP
