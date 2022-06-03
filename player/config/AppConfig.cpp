#include "AppConfig.hpp"

#include <boost/algorithm/string/predicate.hpp>

#include "common/PlayerRuntimeError.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"

#if !defined(SNAP_ENABLED)
#include "GitHash.hpp"
#endif

#include <filesystem>
#include <linux/limits.h>
#include <unistd.h>

FilePath AppConfig::resourceDirectory_;

std::string AppConfig::version()
{
    return releaseVersion() + "-" + codeVersion();
}

std::string AppConfig::releaseVersion()
{
#if defined(SNAP_ENABLED)
    return getenv("SNAP_VERSION");
#else
    // Update this with each release.
    return std::string{"1.8 R"} + codeVersion() + GIT_HASH;
#endif
}

std::string AppConfig::codeVersion()
{
#if defined(SNAP_ENABLED)
    if (boost::starts_with(getenv("SNAP_REVISION"), "x")) {
        return "7";
    }
    return getenv("SNAP_REVISION");
#else
    // Update this with each release
    return "7";
#endif
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

FilePath AppConfig::configDirectory()
{
#if defined(SNAP_ENABLED)
    return FilePath{getenv("SNAP_USER_COMMON")};
#else
    return execDirectory();
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

FilePath AppConfig::statsCache()
{
    return configDirectory() / "stats.sqlite";
}

FilePath AppConfig::additionalResourcesDirectory()
{
#if defined(SNAP_ENABLED)
    return FilePath{getenv("SNAP")} / "usr" / "share" / "xibo-player";
#else
    return execDirectory();
#endif
}

FilePath AppConfig::splashScreenPath()
{
    return additionalResourcesDirectory() / "splash.jpg";
}

FilePath AppConfig::uiFile()
{
    return additionalResourcesDirectory() / "ui.glade";
}

FilePath AppConfig::execDirectory()
{
#if defined(SNAP_ENABLED)
    return FilePath{getenv("SNAP")} / "usr" / "bin";
#else
    // workaround for those who starts the player out of snap
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    assert(count != -1);
    return FilePath{std::filesystem::path(std::string(result, static_cast<size_t>(count))).parent_path()};
#endif
}

std::string AppConfig::playerBinary()
{
    return (execDirectory() / "xibo-player").string();
}

std::string AppConfig::optionsBinary()
{
    return (execDirectory() / "xibo-options").string();
}
