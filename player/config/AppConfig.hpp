#pragma once

#include "common/fs/FilePath.hpp"

class AppConfig
{
public:
    static std::string version();      // TODO: strong type
    static std::string codeVersion();  // TODO: strong type

    static std::string mainXmrChannel();

    static FilePath resourceDirectory();
    static void resourceDirectory(const FilePath& directory);
    static FilePath buildDirectory();
    static FilePath configDirectory();

#ifdef SNAP_ENABLED
    static FilePath oldConfigDirectory()
    {
        return FilePath{getenv("SNAP_USER_DATA")};
    }
#endif

    static FilePath publicKeyPath();
    static FilePath privateKeyPath();

    static FilePath splashScreenPath();
    static FilePath uiFile();

    static FilePath cmsSettingsPath();
    static FilePath playerSettingsPath();
    static FilePath schedulePath();
    static FilePath cachePath();

    static std::string playerBinary();
    static std::string optionsBinary();

private:
    static FilePath resourceDirectory_;
};
