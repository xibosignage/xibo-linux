#pragma once

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <memory>
#include <signal.h>

#include "SystemTrayIcon.hpp"

class PlayerProcessHandler;

class MainWindow : public Gtk::Window
{
public:
    MainWindow(const Glib::RefPtr<Gtk::Application>& main_app);

    void on_player_started(int process_id);
    void on_player_startup_error(int error_code);
    void on_player_terminated(int status);

private:
    void on_btn_player_started();
    void on_btn_player_terminated();
    void on_btn_player_killed();

    void log_message(const std::string& message);
    void init_buttons();
    void update_buttons();

private:
    Gtk::ScrolledWindow m_scrolled_window;
    Gtk::TextView m_log_view;
    Gtk::Button m_start_player_btn;
    Gtk::Button m_terminate_player_btn;
    Gtk::Button m_kill_player_btn;
    Gtk::Box m_btn_box;
    Gtk::Box m_main_box;

    std::shared_ptr<PlayerProcessHandler> m_player_app;
    std::unique_ptr<SystemTrayIcon> m_system_tray;

};
