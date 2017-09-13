#include "XiboWatchdogApp.hpp"

#include "XiboWatchdogWindow.hpp"

XiboWatchdogApp::XiboWatchdogApp()
{

}

bool XiboWatchdogApp::OnInit()
{
    auto mainWindow = new XiboWatchdogWindow;
    mainWindow->Center();
    mainWindow->Show(true);
    return wxApp::OnInit();
}

int XiboWatchdogApp::OnExit()
{
    return wxApp::OnExit();
}
