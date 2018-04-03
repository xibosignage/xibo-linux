#include "PlayerProcessHandler.hpp"
#include "MainWindow.hpp"

#include <thread>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>

const char* PLAYER_PATH = "./XiboLinux";

PlayerProcessHandler::PlayerProcessHandler(MainWindow* parent_window) :
    m_parent_window(parent_window)
{
}

PlayerProcessHandler::~PlayerProcessHandler()
{
    if(m_is_running)
    {
        stop(SIGTERM);
        join();
    }
}

bool PlayerProcessHandler::is_running()
{
    return m_is_running;
}

void PlayerProcessHandler::create_process()
{
    m_process_id = fork();
    if(m_process_id > 0)
    {
        int status;
        m_is_running = true;
        m_player_started.emit(m_process_id);
        waitpid(-1, &status, 0);
        m_player_terminated.emit(status);
    }
    else
    {
        if(m_process_id == 0)
        {
            char* args[] = {const_cast<char*>(PLAYER_PATH), nullptr};
            execv(PLAYER_PATH, args);
        }
        else
        {
            m_player_startup_error.emit(errno);
        }
    }
}

void PlayerProcessHandler::run()
{
    if(!boost::filesystem::exists(PLAYER_PATH))
    {
        errno = ENOENT;
        m_player_startup_error.emit(errno);
        return;
    }

    auto funct = std::bind(&PlayerProcessHandler::create_process, this);
    m_process_thread = std::thread(funct);
}

void PlayerProcessHandler::stop(int status)
{
    kill(m_process_id, status);
}

void PlayerProcessHandler::join()
{
    if(m_process_thread.joinable())
    {
        m_process_thread.join();
    }
    m_is_running = false;
}
