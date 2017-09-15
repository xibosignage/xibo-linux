#include "WatchdogApp.hpp"
#include "MainWindow.hpp"

WatchdogApp::WatchdogApp()
{

}

bool WatchdogApp::OnInit()
{
    auto mainWindow = new MainWindow;
    mainWindow->Center();
    mainWindow->Show(true);
    SetTopWindow(mainWindow);
    return wxApp::OnInit();
}

int WatchdogApp::OnExit()
{
    return wxApp::OnExit();
}
