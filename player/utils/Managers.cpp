#include "Managers.hpp"
#include "XiboApp.hpp"

FileCacheManager& Managers::fileManager()
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
