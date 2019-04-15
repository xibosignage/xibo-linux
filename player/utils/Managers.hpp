#pragma once

#include "constants.hpp"

class HttpManager;
class FileCacheManager;
class FilePath;
class ScreenShoter;

namespace Managers
{
    FileCacheManager& fileManager();
    ScreenShoter& screenShoter();
    std::string xmlLogs();
}
