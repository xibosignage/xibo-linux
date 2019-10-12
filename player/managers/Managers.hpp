#pragma once

class FileCache;
class XiboWebServer;

namespace Managers
{
    FileCache& fileManager();
    XiboWebServer& webserver();
}
