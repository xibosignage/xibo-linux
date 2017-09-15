#include "PlayerProcessHandler.hpp"
#include "MainWindow.hpp"

#include <thread>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
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
        wxQueueEvent(m_parentWindow, new wxCommandEvent(event));
    }, wxID_ANY);

    Bind(PLAYER_STARTED, [=](wxCommandEvent& event){
        m_parentWindow->LogMessage(event.GetString());
    }, wxID_ANY);

    Bind(PLAYER_STARTED_ERROR, [=](wxCommandEvent& event){
        m_parentWindow->LogMessage(event.GetString());
    }, wxID_ANY);

    m_parentWindow->Bind(PLAYER_TERMINATED, &MainWindow::OnPlayerTerminated, m_parentWindow);
}

PlayerProcessHandler::~PlayerProcessHandler()
{
    Stop();
}

void PlayerProcessHandler::CreateProcess()
{
    // TODO: check for existence
    m_processId = fork();
    if(m_processId > 0)
    {
        PostEvent(PLAYER_STARTED, "Xibo Player started. Process ID: " + wxString::FromDouble(m_processId));
        int status;
        waitpid(-1, &status, 0);
        PostEvent(PLAYER_TERMINATED, "Xibo Player exited with status: " + wxString::FromDouble(status));
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
            PostEvent(PLAYER_STARTED_ERROR, "Can't start Xibo Player. Error code: " + wxString::FromDouble(errno));
        }
    }
}

void PlayerProcessHandler::Run()
{
    auto funct = std::bind(&PlayerProcessHandler::CreateProcess, this);
    m_processThread = std::thread(funct);
}

void PlayerProcessHandler::Stop()
{
    if(m_processThread.joinable())
    {
        kill(m_processId, SIGTERM);
        m_processThread.join();
    }
}

void PlayerProcessHandler::PostEvent(wxEventType type, const wxString& message)
{
    wxCommandEvent event(type);
    event.SetString(message);
    wxPostEvent(this, event);
}
