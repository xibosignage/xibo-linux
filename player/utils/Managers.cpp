#include "Managers.hpp"

#include "XiboApp.hpp"
#include "ScreenShoter.hpp"

FileCacheManager& Managers::fileManager()
{
    return XiboApp::app().fileManager();
}

ScreenShoter& Managers::screenShoter()
{
    return XiboApp::app().screenShoter();
}
