#include "XiboApp.hpp"

#include <wx/mediactrl.h>

XiboApp::XiboApp()
{

}

bool XiboApp::OnInit()
{
    m_mainWindow = new MainWindow(nullptr, wxID_ANY);
    m_mainWindow->Show(true);
    return wxApp::OnInit();
}

int XiboApp::OnExit()
{
    return wxApp::OnExit();
}
