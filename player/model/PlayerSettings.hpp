#pragma once

#include "utils/Field.hpp"
#include "utils/logger/LoggingLevel.hpp"

struct PlayerSettings
{
    Field<int> collectInterval{"collectInterval", 5};
    Field<std::string> downloadStartWindow{"downloadStartWindow", "00:00"};
    Field<std::string> downloadEndWindow{"downloadEndWindow", "00:00"};
    Field<bool> statsEnabled{"statsEnabled", false};
    Field<std::string> xmrNetworkAddress{"xmrNetworkAddress"};
    Field<double> sizeX{"sizeX", 0.0};
    Field<double> sizeY{"sizeY", 0.0};
    Field<double> offsetX{"offsetX", 0.0};
    Field<double> offsetY{"offsetY", 0.0};
    Field<LoggingLevel> logLevel{"logLevel", LoggingLevel::Debug};
    Field<bool> shellCommandsEnabled{"shellCommandsEnabled", false};
    Field<bool> modifiedLayoutsEnabled{"modifiedLayoutsEnabled", false};
    Field<int> maxConcurrentDownloads{"maxConcurrentDownloads", 2};
    Field<bool> statusLayoutUpdate{"statusLayoutUpdate", false};
    Field<int> screenshotInterval{"screenshotInterval", 0};
    Field<int> screenshotSize{"screenshotSize", 0};
    Field<int> maxLogFilesUploads{"maxLogFilesUploads", 1};
    Field<int> embeddedServerPort{"embeddedServerPort", 0};
    Field<bool> preventSleep{"preventSleep", false};
    Field<std::string> displayName{"displayName"};
    Field<bool> screenshotRequested{"screenshotRequested", false};
};
