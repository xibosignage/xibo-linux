#include "MainWindow.hpp"
#include "PlayerProcessHandler.hpp"
#include "SystemTrayIcon.hpp"

#include <glibmm.h>
#include <spdlog/spdlog.h>

const int DEFAULT_RESTART_TIME = 5000;

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& main_app) :
    m_start_player_btn("Start Xibo Player"),
    m_terminate_player_btn("Terminate Xibo Player"),
    m_kill_player_btn("Kill Xibo Player"),
    m_btn_box(Gtk::ORIENTATION_HORIZONTAL, 20),
    m_main_box(Gtk::ORIENTATION_VERTICAL, 10)
{
    set_default_size(640, 480);
    set_border_width(15);
    set_position(Gtk::WIN_POS_CENTER);

    main_app->hold();

    signal_delete_event().connect([=](GdkEventAny*){
        main_app->release();
        return false;
    });

    m_player_app = std::make_shared<PlayerProcessHandler>(this);
    m_system_tray = std::make_unique<SystemTrayIcon>(this, m_player_app);

    m_player_app->signal_player_started().connect(sigc::mem_fun(*this, &MainWindow::on_player_started));
    m_player_app->signal_player_startup_error().connect(sigc::mem_fun(*this, &MainWindow::on_player_startup_error));
    m_player_app->signal_player_terminated().connect(sigc::mem_fun(*this, &MainWindow::on_player_terminated));

    m_scrolled_window.add(m_log_view);
    m_main_box.pack_start(m_scrolled_window);

    init_buttons();

    add(m_main_box);
    show_all();

}

void MainWindow::init_buttons()
{
    m_btn_box.pack_start(m_start_player_btn);
    m_btn_box.pack_start(m_terminate_player_btn);
    m_terminate_player_btn.set_sensitive(false);
    m_btn_box.pack_start(m_kill_player_btn);
    m_kill_player_btn.set_sensitive(false);

    m_start_player_btn.signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_btn_player_started));
    m_terminate_player_btn.signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_btn_player_terminated));
    m_kill_player_btn.signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_btn_player_killed));

    m_main_box.pack_start(m_btn_box, false, false);
}

void MainWindow::log_message(const std::string& message)
{
    auto buffer = m_log_view.get_buffer();
    buffer->insert(buffer->end(), message + "\n");
}

void MainWindow::on_btn_player_started()
{
    if(!m_player_app->is_running())
    {
        m_player_app->run();
    }
}

void MainWindow::on_btn_player_terminated()
{
    if(m_player_app->is_running())
    {
        m_player_app->stop(SIGTERM);
    }
}

void MainWindow::on_btn_player_killed()
{
    if(m_player_app->is_running())
    {
        m_player_app->stop(SIGKILL);
    }
}

void MainWindow::on_player_started(int process_id)
{
    update_buttons();
    log_message("Xibo Player started. Process ID: " + std::to_string(process_id));
}

void MainWindow::on_player_startup_error(int error_code)
{
    log_message("Can't start Xibo Player. Error code: " + std::to_string(error_code));
}

void MainWindow::on_player_terminated(int status)
{
    m_player_app->join();
    update_buttons();
    log_message("Xibo Player exited with status: " + std::to_string(status));
    if(status != SIGTERM && status != 0)
    {
        log_message("Player will be restarted in " + std::to_string(DEFAULT_RESTART_TIME / 1000) + " seconds");
        Glib::signal_timeout().connect_once([=](){
            if(!m_player_app->is_running())
            {
                m_player_app->run();
            }
        }, DEFAULT_RESTART_TIME);
    }
}

void MainWindow::update_buttons()
{
    m_start_player_btn.set_sensitive(!m_player_app->is_running());
    m_terminate_player_btn.set_sensitive(m_player_app->is_running());
    m_kill_player_btn.set_sensitive(m_player_app->is_running());
}
