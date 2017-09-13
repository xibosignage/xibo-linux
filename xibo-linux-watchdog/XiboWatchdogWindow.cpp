#include "XiboWatchdogWindow.hpp"

#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

XiboWatchdogWindow::XiboWatchdogWindow() :
    wxFrame(nullptr, wxID_ANY, "Xibo Watchdog", wxDefaultPosition, wxSize(640, 480))
{
    m_mainSizer = new wxBoxSizer(wxVERTICAL);

    InitMenuBar();
    InitLogEdit();
    InitButtons();

    SetSizer(m_mainSizer);
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

void XiboWatchdogWindow::OnAppOpened(wxCommandEvent& WXUNUSED(event))
{
    pid_t processId = fork();
    if(processId > 0)
    {
        LogMessage("Xibo Player started. Process ID: " + wxString::FromDouble(processId));
        int status;
        waitpid(-1, &status, 0);
        LogMessage("Xibo Player exit with status: " + wxString::FromDouble(status));
    }
    else
    {
        if(processId == 0)
        {
            char* args[] = {"./xibo-linux", NULL};
            execv("./xibo-linux", args);
            std::cout << "now child" << errno << std::endl;
        }
        else
        {
            LogMessage("Can't start Xibo Player. Error code: " + wxString::FromDouble(errno));
        }
    }
}

void XiboWatchdogWindow::OnLogAdded(wxCommandEvent& WXUNUSED(event))
{
    m_logEdit->AppendText("Testing very loooooooooooooooooooooooooooooooooong message\n");
}
