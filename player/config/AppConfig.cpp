#include "AppConfig.hpp"

#include "common/PlayerRuntimeError.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"

#include <filesystem>
#include <linux/limits.h>
#include <unistd.h>

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
    // workaround for those who starts the player out of snap
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    assert(count != -1);
    return FilePath{std::filesystem::path(std::string(result, static_cast<size_t>(count))).parent_path()};
#endif
}

FilePath AppConfig::configDirectory()
{
#ifdef SNAP_ENABLED
    return FilePath{getenv("SNAP_USER_COMMON")};
#else
    return buildDirectory();
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
