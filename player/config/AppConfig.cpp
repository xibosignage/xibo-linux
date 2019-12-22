#include "AppConfig.hpp"

#include "common/PlayerRuntimeError.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"

FilePath AppConfig::resourceDirectory_;

std::string AppConfig::version()
{
#ifdef SNAP_ENABLED
    return getenv("SNAP_VERSION");
#else
    return std::string{"dev-version"};
#endif
}

std::string AppConfig::codeVersion()
{
#ifdef SNAP_ENABLED
    return getenv("SNAP_REVISION");
#else
    return std::string{"dev-revision"};
#endif
}

std::string AppConfig::mainXmrChannel()
{
    return "playerLinux";
}

FilePath AppConfig::resourceDirectory()
{
    return resourceDirectory_;
}

void AppConfig::resourceDirectory(const FilePath& directory)
{
    if (!FileSystem::exists(directory))
        throw PlayerRuntimeError{
            "AppConfig", "Resource directory doesn't exist. Create or use exsiting one in the player options app."};

    resourceDirectory_ = directory;
}

FilePath AppConfig::buildDirectory()
{
#ifdef SNAP_ENABLED
    return FilePath{getenv("SNAP")} / "bin";
#else
    return FileSystem::currentPath();
#endif
}

FilePath AppConfig::configDirectory()
{
#ifdef SNAP_ENABLED
    return FilePath{getenv("SNAP_USER_COMMON")};
#else
    return FileSystem::currentPath();
#endif
}

FilePath AppConfig::publicKeyPath()
{
    return configDirectory() / "id_rsa.pub";
}

FilePath AppConfig::privateKeyPath()
{
    return configDirectory() / "id_rsa";
}

FilePath AppConfig::splashScreenPath()
{
    return buildDirectory() / "splash.jpg";
}

FilePath AppConfig::uiFile()
{
    return buildDirectory() / "ui.glade";
}

FilePath AppConfig::cmsSettingsPath()
{
    return configDirectory() / "cmsSettings.xml";
}

FilePath AppConfig::playerSettingsPath()
{
    return configDirectory() / "playerSettings.xml";
}

FilePath AppConfig::schedulePath()
{
    return configDirectory() / "schedule.xml";
}

FilePath AppConfig::cachePath()
{
    return configDirectory() / "cacheFile.xml";
}

std::string AppConfig::playerBinary()
{
    return (buildDirectory() / "player").string();
}

std::string AppConfig::optionsBinary()
{
    return (buildDirectory() / "options").string();
}
