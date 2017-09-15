#include "MainWindow.hpp"
#include "PlayerProcessHandler.hpp"
#include "SystemTrayIcon.hpp"

#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>

#include <iostream>

const int DEFAULT_RESTART_TIME = 5000;

MainWindow::MainWindow() :
    wxFrame(nullptr, wxID_ANY, "Xibo Watchdog", wxDefaultPosition, wxSize(640, 480)),
    m_restartTimer(this)
{
    m_playerApp = std::make_shared<PlayerProcessHandler>(this);
    m_systemTrayIcon = std::make_unique<SystemTrayIcon>(this, m_playerApp);
    m_mainSizer = new wxBoxSizer(wxVERTICAL);

    Bind(wxEVT_TIMER, &MainWindow::OnRestartTimerFinished, this);

    InitMenuBar();
    InitLogEdit();
    InitButtons();

    SetSizer(m_mainSizer);
}

void MainWindow::InitMenuBar()
{
    m_menuBar = new wxMenuBar;
    m_menu = new wxMenu;

    m_menu->Append(wxID_HELP, _T("&Help"));
    m_menu->Append(wxID_CLOSE, _T("&Close"));
    m_menuBar->Append(m_menu, _T("&About"));

    SetMenuBar(m_menuBar);
}

void MainWindow::InitLogEdit()
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


void MainWindow::InitButtons()
{
    m_startPlayerBtn = new wxButton(this, wxID_ANY, "Start Xibo Player");
    m_terminatePlayerBtn = new wxButton(this, wxID_ANY, "Terminate Xibo Player");
    m_terminatePlayerBtn->Disable();
    m_killPlayerBtn = new wxButton(this, wxID_ANY, "Kill Xibo Player");
    m_killPlayerBtn->Disable();
    m_btnSizer = new wxBoxSizer(wxHORIZONTAL);

    m_startPlayerBtn->Bind(wxEVT_BUTTON, &MainWindow::OnBtnPlayerStart, this);
    m_terminatePlayerBtn->Bind(wxEVT_BUTTON, &MainWindow::OnBtnPlayerTerminated, this);
    m_killPlayerBtn->Bind(wxEVT_BUTTON, &MainWindow::OnBtnPlayerKilled, this);

    m_btnSizer->Add(m_startPlayerBtn);
    m_btnSizer->AddSpacer(20);
    m_btnSizer->Add(m_terminatePlayerBtn);
    m_btnSizer->AddSpacer(20);
    m_btnSizer->Add(m_killPlayerBtn);

    m_mainSizer->Add(m_btnSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
}

void MainWindow::LogMessage(const wxString& logMessage)
{
    m_logEdit->AppendText(logMessage + "\n");
}

void MainWindow::OnRestartTimerFinished(wxTimerEvent& WXUNUSED(event))
{
    if(!m_playerApp->isRunning())
    {
        m_playerApp->Run();
    }
}

void MainWindow::OnBtnPlayerStart(wxCommandEvent& WXUNUSED(event))
{
    if(!m_playerApp->isRunning())
    {
        m_playerApp->Run();
    }
}

void MainWindow::OnBtnPlayerTerminated(wxCommandEvent& WXUNUSED(event))
{
    if(m_playerApp->isRunning())
    {
        m_playerApp->Stop(SIGTERM);
    }
}

void MainWindow::OnBtnPlayerKilled(wxCommandEvent& WXUNUSED(event))
{
    if(m_playerApp->isRunning())
    {
        m_playerApp->Stop(SIGKILL);
    }
}

void MainWindow::OnPlayerStarted(wxCommandEvent& event)
{
    UpdateButtons();
    LogMessage("Xibo Player started. Process ID: " + wxString::FromDouble(event.GetInt()));
}

void MainWindow::OnPlayerStartedError(wxCommandEvent& event)
{
    LogMessage("Can't start Xibo Player. Error code: " + wxString::FromDouble(event.GetInt()));
}

void MainWindow::OnPlayerClosed(wxCommandEvent& event)
{
    UpdateButtons();
    LogMessage("Xibo Player exited with status: " + wxString::FromDouble(event.GetInt()));
    if(event.GetInt() != SIGTERM && event.GetInt() != 0)
    {
        LogMessage("Player will be restarted in " + wxString::FromDouble(DEFAULT_RESTART_TIME / 1000) + " seconds");
        m_restartTimer.StartOnce(DEFAULT_RESTART_TIME);
    }
}

void MainWindow::UpdateButtons()
{
    m_startPlayerBtn->Enable(!m_playerApp->isRunning());
    m_terminatePlayerBtn->Enable(m_playerApp->isRunning());
    m_killPlayerBtn->Enable(m_playerApp->isRunning());
}
