#pragma once

#include "common/NamedField.hpp"
#include "common/Parsing.hpp"

class FilePath;

struct PlayerSettings
{
    void fromFile(const FilePath& file);
    void fromNode(const XmlNode& node);
    void saveTo(const FilePath& file);

    NamedField<int> collectInterval{"collectInterval", 900};
    NamedField<std::string> downloadStartWindow{"downloadStartWindow", "00:00"};
    NamedField<std::string> downloadEndWindow{"downloadEndWindow", "00:00"};
    NamedField<bool> statsEnabled{"statsEnabled", false};
    NamedField<std::string> aggregationLevel{"aggregationLevel", "Individual"};
    NamedField<std::string> xmrNetworkAddress{"xmrNetworkAddress"};
    NamedField<int> width{"sizeX", 0};
    NamedField<int> height{"sizeY", 0};
    NamedField<int> x{"offsetX", 0};
    NamedField<int> y{"offsetY", 0};
    NamedField<std::string> logLevel{"logLevel", "debug"};
    NamedField<bool> shellCommandsEnabled{"shellCommandsEnabled", false};
    NamedField<bool> modifiedLayoutsEnabled{"modifiedLayoutsEnabled", false};
    NamedField<int> maxConcurrentDownloads{"maxConcurrentDownloads", 2};
    NamedField<bool> statusLayoutUpdate{"statusLayoutUpdate", false};
    NamedField<int> screenshotInterval{"screenshotInterval", 0};
    NamedField<int> screenshotSize{"screenshotSize", 0};
    NamedField<int> maxLogFilesUploads{"maxLogFilesUploads", 1};
    NamedField<unsigned short> embeddedServerPort{"embeddedServerPort", 1234};
    NamedField<bool> preventSleep{"preventSleep", false};
    NamedField<std::string> displayName{"displayName"};
    NamedField<bool> screenshotRequested{"screenshotRequested", false};
};

PlayerSettings& playerSettings();
void updatePlayerSettings(const PlayerSettings& settings);
