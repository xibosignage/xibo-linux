#include "Managers.hpp"
#include "XiboApp.hpp"

FileCache& Managers::fileManager()
{
    return xiboApp().fileManager();
}

ScreenShoter& Managers::screenShoter()
{
    return xiboApp().screenShoter();
}

XiboWebServer& Managers::webserver()
{
    return xiboApp().webserver();
}
