#pragma once

class HttpClient;
class FileCache;
class ScreenShoter;
class XiboWebServer;

namespace Managers
{
    FileCache& fileManager();
    ScreenShoter& screenShoter();
    XiboWebServer& webserver();
}
