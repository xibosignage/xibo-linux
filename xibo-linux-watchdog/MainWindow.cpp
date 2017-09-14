#include "MainWindow.hpp"
#include "PlayerProcessHandler.hpp"

#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>

MainWindow::MainWindow() :
    wxFrame(nullptr, wxID_ANY, "Xibo Watchdog", wxDefaultPosition, wxSize(640, 480))
{
    m_playerApp = std::make_unique<PlayerProcessHandler>(this);
    m_mainSizer = new wxBoxSizer(wxVERTICAL);

    InitMenuBar();
    InitLogEdit();
    InitButtons();

    SetSizer(m_mainSizer);
}

void MainWindow::InitMenuBar()
{
    auto menuBar = new wxMenuBar;
    auto menu = new wxMenu();

    menu->Append(wxID_HELP, _T("&Help"));
    menu->Append(wxID_CLOSE, _T("&Close"));
    menuBar->Append(menu, _T("&About"));

    SetMenuBar(menuBar);
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
    m_startPlayerBtn = new wxButton(this, wxID_OPEN, "Start Xibo Player");
    m_stopPlayerBtn = new wxButton(this, wxID_STOP, "Stop Xibo Player");
    m_stopPlayerBtn->Disable();
    m_btnSizer = new wxBoxSizer(wxHORIZONTAL);

    Bind(wxEVT_BUTTON, &MainWindow::OnAppStarted, this, wxID_OPEN);
    Bind(wxEVT_BUTTON, &MainWindow::OnAppStopped, this, wxID_STOP);

    m_btnSizer->Add(m_startPlayerBtn);
    m_btnSizer->AddSpacer(20);
    m_btnSizer->Add(m_stopPlayerBtn);

    m_mainSizer->Add(m_btnSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
}

void MainWindow::LogMessage(const wxString& logMessage)
{
    m_logEdit->AppendText(logMessage + "\n");
}

void MainWindow::OnAppStopped(wxCommandEvent& WXUNUSED(event))
{
    m_playerApp->Stop();
}

void MainWindow::OnAppStarted(wxCommandEvent& WXUNUSED(event))
{
    m_startPlayerBtn->Disable();
    m_stopPlayerBtn->Enable();
    m_playerApp->Run();
}

void MainWindow::OnPlayerTerminated(wxCommandEvent& event)
{
    m_startPlayerBtn->Enable();
    m_stopPlayerBtn->Disable();
    LogMessage(event.GetString());
}
