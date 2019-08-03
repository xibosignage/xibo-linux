#include "Managers.hpp"
#include "XiboApp.hpp"

FileCache& Managers::fileManager()
{
    return XiboApp::app().fileManager();
}

ScreenShoter& Managers::screenShoter()
{
    return XiboApp::app().screenShoter();
}

XiboWebServer& Managers::webserver()
{
    return XiboApp::app().webserver();
}
