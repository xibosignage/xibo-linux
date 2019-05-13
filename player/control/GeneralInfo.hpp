#pragma once

#include <boost/date_time/posix_time/ptime.hpp>

struct GeneralInfo
{
    boost::posix_time::ptime currentDT;
    std::string projectVersion;
    int codeVersion;
    std::string cmsAddress;
    std::string resourcesPath;
    std::string displayName;
    int windowWidth;
    int windowHeight;
    int screenShotInterval;
    int numberOfLogsToSend;
};
