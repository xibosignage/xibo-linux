#pragma once

#include "common/Field.hpp"
#include "common/Parsing.hpp"

class FilePath;

struct PlayerSettings
{
    void fromFile(const FilePath& file);
    void fromNode(const XmlNode& node);
    void saveTo(const FilePath& file);

    Field<int> collectInterval{"collectInterval", 900};
    Field<std::string> downloadStartWindow{"downloadStartWindow", "00:00"};
    Field<std::string> downloadEndWindow{"downloadEndWindow", "00:00"};
    Field<bool> statsEnabled{"statsEnabled", false};
    Field<std::string> aggregationLevel{"aggregationLevel", "Individual"};
    Field<std::string> xmrNetworkAddress{"xmrNetworkAddress"};
    Field<int> width{"sizeX", 0};
    Field<int> height{"sizeY", 0};
    Field<int> x{"offsetX", 0};
    Field<int> y{"offsetY", 0};
    Field<std::string> logLevel{"logLevel", "debug"};
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

PlayerSettings& playerSettings();
void updatePlayerSettings(const PlayerSettings& settings);
