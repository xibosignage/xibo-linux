#include "XiboApp.hpp"

#include <wx/mediactrl.h>

XiboApp::XiboApp()
{

}

bool XiboApp::OnInit()
{
    m_mainWindow = new MainWindow(nullptr, wxID_ANY);
    // m_mainWindow->GetWindowStyle() & ~wxSTAY_ON_TOP to remove it from top
    m_mainWindow->SetWindowStyle(m_mainWindow->GetWindowStyle() | wxSTAY_ON_TOP);
    // ShowFullScreen(true) to run in full-screen mode
    m_mainWindow->Show(true);
    return wxApp::OnInit();
}

int XiboApp::OnExit()
{
    return wxApp::OnExit();
}
