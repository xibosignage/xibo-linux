#pragma once

#include "common/dt/DateTime.hpp"
#include "common/fs/FilePath.hpp"

struct GeneralInfo
{
    DateTime currentDateTime;
    // TODO: strong type
    std::string projectVersion;
    std::string codeVersion;
    std::string cmsAddress;
    FilePath resourcesPath;
    std::string displayName;
    int windowWidth;
    int windowHeight;
    int screenShotInterval;
    int numberOfLogsToSend;
};
