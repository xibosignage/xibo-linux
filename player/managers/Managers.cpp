#include "Managers.hpp"
#include "XiboApp.hpp"

FileCache& Managers::fileManager()
{
    return xiboApp().fileManager();
}

XiboWebServer& Managers::webserver()
{
    return xiboApp().webserver();
}
