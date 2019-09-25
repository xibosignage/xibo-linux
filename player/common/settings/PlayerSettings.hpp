#pragma once

#include "../Field.hpp"
#include "../logger/LoggingLevel.hpp"
#include "constants.hpp"

class FilePath;

class PlayerSettings
{
public:
    struct Dimensions
    {
        int width;
        int height;
        int x;
        int y;
    };

    void loadFrom(const FilePath& file);
    void loadFrom(const ptree_node& node);
    void saveTo(const FilePath& file);

    Field<int> collectInterval{"collectInterval", 900};
    Field<std::string> downloadStartWindow{"downloadStartWindow", "00:00"};
    Field<std::string> downloadEndWindow{"downloadEndWindow", "00:00"};
    Field<bool> statsEnabled{"statsEnabled", false};
    Field<std::string> xmrNetworkAddress{"xmrNetworkAddress"};
    Field<int> width{"sizeX", 0};
    Field<int> height{"sizeY", 0};
    Field<int> x{"offsetX", 0};
    Field<int> y{"offsetY", 0};
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
