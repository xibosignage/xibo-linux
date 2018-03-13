#ifndef XIBOPLAYERAPP_HPP
#define XIBOPLAYERAPP_HPP

#include <thread>
#include "Dispatcher.hpp"

class MainWindow;

class PlayerProcessHandler
{
public:
    PlayerProcessHandler(MainWindow* parent_window);
    ~PlayerProcessHandler();

    bool is_running();
    void run();
    void stop(int status);
    void join();

    Dispatcher<int>& signal_player_started() { return m_player_started; }
    Dispatcher<int>& signal_player_startup_error() { return m_player_startup_error; }
    Dispatcher<int>& signal_player_terminated() { return m_player_terminated; }

private:
    void create_process();

private:
    MainWindow* m_parent_window = nullptr;
    std::thread m_process_thread;
    bool m_is_running = false;
    pid_t m_process_id;

    Dispatcher<int> m_player_started;
    Dispatcher<int> m_player_startup_error;
    Dispatcher<int> m_player_terminated;

};

#endif // XIBOPLAYERAPP_HPP
