#pragma once

#include "common/dt/DateTime.hpp"

struct GeneralInfo
{
    DateTime currentDT;
    std::string projectVersion;
    std::string codeVersion;
    std::string cmsAddress;
    std::string resourcesPath;
    std::string displayName;
    int windowWidth;
    int windowHeight;
    int screenShotInterval;
    int numberOfLogsToSend;
};
