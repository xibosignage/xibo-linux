#pragma once

#include "common/Field.hpp"
#include "common/logger/LoggingLevel.hpp"

struct PlayerSettings
{
    Field<int> collectInterval{"collectInterval", 900};
    Field<std::string> downloadStartWindow{"downloadStartWindow", "00:00"};
    Field<std::string> downloadEndWindow{"downloadEndWindow", "00:00"};
    Field<bool> statsEnabled{"statsEnabled", false};
    Field<std::string> xmrNetworkAddress{"xmrNetworkAddress"};

    struct Dimensions
    {
        Field<int> width{"sizeX", 0};
        Field<int> height{"sizeY", 0};
        Field<int> x{"offsetX", 0};
        Field<int> y{"offsetY", 0};
    } dimensions;

    Field<LoggingLevel> logLevel{"logLevel", LoggingLevel::Debug};
    Field<bool> shellCommandsEnabled{"shellCommandsEnabled", false};
    Field<bool> modifiedLayoutsEnabled{"modifiedLayoutsEnabled", false};
    Field<int> maxConcurrentDownloads{"maxConcurrentDownloads", 2};
    Field<bool> statusLayoutUpdate{"statusLayoutUpdate", false};
    Field<int> screenshotInterval{"screenshotInterval", 0};
    Field<int> screenshotSize{"screenshotSize", 0};
    Field<int> maxLogFilesUploads{"maxLogFilesUploads", 1};
    Field<unsigned short> embeddedServerPort{"embeddedServerPort", 1234};
    Field<bool> preventSleep{"preventSleep", false};
    Field<std::string> displayName{"displayName"};
    Field<bool> screenshotRequested{"screenshotRequested", false};
};
