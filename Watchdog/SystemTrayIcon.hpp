#ifndef SYSTEMTRAYICON_HPP
#define SYSTEMTRAYICON_HPP

#include <memory>
#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>

class MainWindow;
class PlayerProcessHandler;

class SystemTrayIcon : public Gtk::StatusIcon
{
public:
    SystemTrayIcon(MainWindow* parent_window,
                   const std::shared_ptr<PlayerProcessHandler>& player_app);

private:
    void create_popup_menu();
    void update_popup_menu();

    void connect_main_tray_menu();
    void connect_close_menu();

private:
    Gtk::Menu m_tray_menu;
    Gtk::MenuItem m_close_player_item;
    Gtk::MenuItem m_start_player_item;
    Gtk::MenuItem m_hide_watchdog_item;
    Gtk::MenuItem m_show_watchdog_item;
    Gtk::MenuItem m_close_watchdog_item;

    Gtk::Menu m_close_player_menu;
    Gtk::MenuItem m_terminate_player_item;
    Gtk::MenuItem m_kill_player_item;

    MainWindow* m_parent_window = nullptr;
    std::shared_ptr<PlayerProcessHandler> m_player_app;

};

#endif // SYSTEMTRAYICON_HPP
