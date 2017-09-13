#include "XiboWatchdogWindow.hpp"

#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>

#include <thread>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

wxDEFINE_EVENT(XIBO_PLAYER_TERMINATED, wxCommandEvent);

XiboWatchdogWindow::XiboWatchdogWindow() :
    wxFrame(nullptr, wxID_ANY, "Xibo Watchdog", wxDefaultPosition, wxSize(640, 480))
{
    m_mainSizer = new wxBoxSizer(wxVERTICAL);

    InitMenuBar();
    InitLogEdit();
    InitButtons();

    Bind(XIBO_PLAYER_TERMINATED, [=](wxCommandEvent& event){
        LogMessage(event.GetString());
        m_processThread.join();
    }, wxID_ANY);

    SetSizer(m_mainSizer);
}

XiboWatchdogWindow::~XiboWatchdogWindow()
{
    if(m_processThread.joinable())
    {
        kill(m_processId, SIGTERM);
        m_processThread.join();
    }
}

void XiboWatchdogWindow::InitMenuBar()
{
    auto menuBar = new wxMenuBar;
    auto menu = new wxMenu();

    menu->Append(wxID_HELP, _T("&Help"));
    menu->Append(wxID_CLOSE, _T("&Close"));
    menuBar->Append(menu, _T("&About"));

    SetMenuBar(menuBar);
}

void XiboWatchdogWindow::InitLogEdit()
{
    m_logEdit = new wxTextCtrl(this,
                               wxID_ANY,
                               wxString{},
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxTE_MULTILINE);
    m_mainSizer->Add(m_logEdit, 1, wxEXPAND | wxALL, 5);
    m_mainSizer->AddSpacer(15);
}

void XiboWatchdogWindow::InitButtons()
{
    auto clearBtn = new wxButton(this, wxID_OPEN, "Start Xibo Player");
    auto exampleLogBtn = new wxButton(this, wxID_ADD, "Add example log");
    m_btnSizer = new wxBoxSizer(wxHORIZONTAL);

    Bind(wxEVT_BUTTON, &XiboWatchdogWindow::OnAppOpened, this, wxID_OPEN);
    Bind(wxEVT_BUTTON, &XiboWatchdogWindow::OnLogAdded, this, wxID_ADD);

    m_btnSizer->Add(clearBtn);
    m_btnSizer->AddSpacer(20);
    m_btnSizer->Add(exampleLogBtn);

    m_mainSizer->Add(m_btnSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
}

void XiboWatchdogWindow::LogMessage(const wxString& logMessage)
{
    m_logEdit->AppendText(logMessage + "\n");
}

void XiboWatchdogWindow::CreateProcess()
{
    m_processId = fork();
    if(m_processId > 0)
    {
        LogMessage("Xibo Player started. Process ID: " + wxString::FromDouble(m_processId));
        int status;
        waitpid(-1, &status, 0);
        wxCommandEvent* event = new wxCommandEvent(XIBO_PLAYER_TERMINATED);
        event->SetString("Xibo Player exited with status: " + wxString::FromDouble(status));
        wxQueueEvent(this, event);
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
            LogMessage("Can't start Xibo Player. Error code: " + wxString::FromDouble(errno));
        }
    }
}

void XiboWatchdogWindow::OnAppOpened(wxCommandEvent& WXUNUSED(event))
{
    auto funct = std::bind(&XiboWatchdogWindow::CreateProcess, this);
    m_processThread = std::thread(funct);
}

void XiboWatchdogWindow::OnLogAdded(wxCommandEvent& WXUNUSED(event))
{
    m_logEdit->AppendText("Testing very loooooooooooooooooooooooooooooooooong message\n");
}
