#include "PlayerProcessHandler.hpp"
#include "MainWindow.hpp"

#include <thread>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

wxDEFINE_EVENT(PLAYER_TERMINATED, wxCommandEvent);
wxDEFINE_EVENT(PLAYER_STARTED, wxCommandEvent);
wxDEFINE_EVENT(PLAYER_STARTED_ERROR, wxCommandEvent);

PlayerProcessHandler::PlayerProcessHandler(MainWindow* parentWindow) :
    m_parentWindow(parentWindow)
{
    Bind(PLAYER_TERMINATED, [=](wxCommandEvent& event){
        if(m_processThread.joinable())
        {
           m_processThread.join();
        }
        m_isRunning = false;
        wxQueueEvent(m_parentWindow, new wxCommandEvent(event));
    });

    Bind(PLAYER_STARTED, [=](wxCommandEvent& event){
        m_isRunning = true;
        wxQueueEvent(m_parentWindow, new wxCommandEvent(event));
    });

    Bind(PLAYER_STARTED_ERROR, [=](wxCommandEvent& event){
        wxQueueEvent(m_parentWindow, new wxCommandEvent(event));
    });

    m_parentWindow->Bind(PLAYER_TERMINATED, &MainWindow::OnPlayerClosed, m_parentWindow);
    m_parentWindow->Bind(PLAYER_STARTED, &MainWindow::OnPlayerStarted, m_parentWindow);
    m_parentWindow->Bind(PLAYER_STARTED_ERROR, &MainWindow::OnPlayerStartedError, m_parentWindow);
}

PlayerProcessHandler::~PlayerProcessHandler()
{
    Stop(SIGTERM);
}

bool PlayerProcessHandler::isRunning()
{
    return m_isRunning;
}

void PlayerProcessHandler::CreateProcess()
{
    // TODO: check for existence
    m_processId = fork();
    if(m_processId > 0)
    {
        int status;
        PostProcessEvent(PLAYER_STARTED, m_processId);
        waitpid(-1, &status, 0);
        PostProcessEvent(PLAYER_TERMINATED, status);
    }
    else
    {
        if(m_processId == 0)
        {
            char* args[] = {const_cast<char*>("./xibo-linux"), NULL};
            execv("./xibo-linux", args);
        }
        else
        {
            PostProcessEvent(PLAYER_STARTED_ERROR, errno);
        }
    }
}

void PlayerProcessHandler::Run()
{
    auto funct = std::bind(&PlayerProcessHandler::CreateProcess, this);
    m_processThread = std::thread(funct);
}

void PlayerProcessHandler::Stop(int status)
{
    if(m_processThread.joinable())
    {
        kill(m_processId, status);
        m_processThread.join();
    }
}

void PlayerProcessHandler::PostProcessEvent(wxEventType type, int value)
{
    wxCommandEvent event(type);
    event.SetInt(value);
    wxPostEvent(this, event);
}
