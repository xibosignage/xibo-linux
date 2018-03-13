#include "SystemTrayIcon.hpp"
#include "MainWindow.hpp"
#include "PlayerProcessHandler.hpp"

#include <spdlog/spdlog.h>

SystemTrayIcon::SystemTrayIcon(MainWindow* parent_window,
                               const std::shared_ptr<PlayerProcessHandler>& player_app) :
    m_parent_window(parent_window),
    m_player_app(player_app)
{
    set_from_file("tray.png");

    create_popup_menu();
    signal_popup_menu().connect([=](guint button, guint32 activate_time){
        update_popup_menu();
        popup_menu_at_position(m_tray_menu, button, activate_time);
    });
}

void SystemTrayIcon::create_popup_menu()
{
    m_terminate_player_item.set_label("Terminate player");
    m_kill_player_item.set_label("Kill player");

    m_close_player_menu.append(m_terminate_player_item);
    m_close_player_menu.append(m_kill_player_item);

    m_start_player_item.set_label("Start player");
    m_start_player_item.set_sensitive(!m_player_app->is_running());
    m_close_player_item.set_label("Close player");
    m_close_player_item.set_sensitive(m_player_app->is_running());
    m_close_player_item.set_submenu(m_close_player_menu);
    m_show_watchdog_item.set_label("Show watchdog");
    m_show_watchdog_item.set_sensitive(!m_parent_window->is_visible());
    m_hide_watchdog_item.set_label("Hide watchdog");
    m_hide_watchdog_item.set_sensitive(m_parent_window->is_visible());
    m_close_watchdog_item.set_label("Close watchdog");

    m_tray_menu.append(m_start_player_item);
    m_tray_menu.append(m_close_player_item);
    m_tray_menu.append(m_show_watchdog_item);
    m_tray_menu.append(m_hide_watchdog_item);
    m_tray_menu.append(m_close_watchdog_item);

    connect_close_menu();
    connect_main_tray_menu();

    m_tray_menu.show_all();
}

void SystemTrayIcon::update_popup_menu()
{
    m_start_player_item.set_sensitive(!m_player_app->is_running());
    m_close_player_item.set_sensitive(m_player_app->is_running());
    m_show_watchdog_item.set_sensitive(!m_parent_window->is_visible());
    m_hide_watchdog_item.set_sensitive(m_parent_window->is_visible());
}

void SystemTrayIcon::connect_close_menu()
{
    m_terminate_player_item.signal_activate().connect([=](){
        if(m_player_app->is_running())
        {
            m_player_app->stop(SIGTERM);
        }
    });

    m_kill_player_item.signal_activate().connect([=](){
        if(m_player_app->is_running())
        {
            m_player_app->stop(SIGKILL);
        }
    });
}

void SystemTrayIcon::connect_main_tray_menu()
{
    m_start_player_item.signal_activate().connect([=](){
        if(!m_player_app->is_running())
        {
            m_player_app->run();
        }
    });

    m_show_watchdog_item.signal_activate().connect([=](){
        m_parent_window->show();
    });

    m_hide_watchdog_item.signal_activate().connect([=](){
        m_parent_window->hide();
    });

    m_close_watchdog_item.signal_activate().connect([=](){
        m_parent_window->close();
    });
}
