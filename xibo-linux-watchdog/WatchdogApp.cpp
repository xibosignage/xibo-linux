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
    return wxApp::OnInit();
}

int WatchdogApp::OnExit()
{
    return wxApp::OnExit();
}
