#pragma once

#include <string>

#include "fs/FilePath.hpp"
#include "fs/FileSystem.hpp"

namespace ProjectResources
{
    // TODO: strong type
    inline std::string version()
    {
#ifdef SNAP_ENABLED
        return getenv("SNAP_VERSION");
#else
        return std::string{"dev-version"};
#endif
    }

    // TODO: strong type
    inline std::string codeVersion()
    {
#ifdef SNAP_ENABLED
        return getenv("SNAP_REVISION");
#else
        return std::string{"dev-revision"};
#endif
    }

    const std::string BuildDir = "bin";
    inline FilePath buildDirectory()
    {
#ifdef SNAP_ENABLED
        return FilePath{getenv("SNAP")} / BuildDir;
#else
        return FileSystem::currentPath();
#endif
    }

    inline FilePath configDirectory()
    {
#ifdef SNAP_ENABLED
        return FilePath{getenv("SNAP_USER_COMMON")};
#else
        return FileSystem::currentPath();
#endif
    }

#ifdef SNAP_ENABLED
    inline FilePath oldConfigDirectory()
    {
        return FilePath{getenv("SNAP_USER_DATA")};
    }
#endif

    const std::string PublicKeyFile = "id_rsa.pub";
    inline FilePath publicKeyPath()
    {
        return configDirectory() / PublicKeyFile;
    }

    const std::string PrivateKeyFile = "id_rsa";
    inline FilePath privateKeyPath()
    {
        return configDirectory() / PrivateKeyFile;
    }

    const std::string ResourcesDir = "resources";
    inline FilePath defaultResourcesDir()
    {
        return configDirectory() / ResourcesDir;
    }

    const std::string SplashImageFile = "splash.jpg";
    inline FilePath splashScreenPath()
    {
        return buildDirectory() / SplashImageFile;
    }

    const std::string UiGladeFile = "ui.glade";
    inline FilePath uiFile()
    {
        return buildDirectory() / UiGladeFile;
    }

    const std::string CmsSettingsFile = "cmsSettings.xml";
    inline FilePath cmsSettingsPath()
    {
        return configDirectory() / CmsSettingsFile;
    }

    const std::string PlayerSettingsFile = "playerSettings.xml";
    inline FilePath playerSettingsPath()
    {
        return configDirectory() / PlayerSettingsFile;
    }

    const std::string ScheduleFile = "schedule.xml";
    inline FilePath schedulePath()
    {
        return configDirectory() / ScheduleFile;
    }

    const std::string CacheFile = "cacheFile.xml";
    inline FilePath cachePath()
    {
        return configDirectory() / CacheFile;
    }

    const std::string PlayerBin = "player";
    inline std::string playerBinary()
    {
        return (buildDirectory() / PlayerBin).string();
    }

    const std::string OptionsBin = "options";
    inline std::string optionsBinary()
    {
        return (buildDirectory() / OptionsBin).string();
    }
}