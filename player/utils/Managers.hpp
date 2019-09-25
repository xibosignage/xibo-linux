#pragma once

#include "constants.hpp"

class HttpClient;
class FileCache;
class FilePath;
class ScreenShoter;
class XiboWebServer;

namespace Managers
{
FileCache& fileManager();
ScreenShoter& screenShoter();
XiboWebServer& webserver();
std::string xmlLogs();
}
