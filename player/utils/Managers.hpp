#pragma once

#include "constants.hpp"

class HttpClient;
class FileCacheManager;
class FilePath;
class ScreenShoter;
class XiboWebServer;

namespace Managers
{
    FileCacheManager& fileManager();
    ScreenShoter& screenShoter();
    XiboWebServer& webserver();
    std::string xmlLogs();
}
