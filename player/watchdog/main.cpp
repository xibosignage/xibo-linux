#include <boost/process/child.hpp>
#include <iostream>

#include "common/fs/FileSystem.hpp"
#include "config.hpp"

#include "common/settings/CmsSettings.hpp"

void setupNewConfigDir()
{
#ifdef SNAP_ENABLED
    try
    {
        using namespace ProjectResources;

        if (FileSystem::exists(oldConfigDirectory() / CmsSettingsFile))
        {
            CmsSettings settings;
            settings.loadFrom(oldConfigDirectory() / CmsSettingsFile);

            std::vector<std::string> filesToMove{CmsSettingsFile, PlayerSettingsFile, PrivateKeyFile, PublicKeyFile};
            for (auto&& file : filesToMove)
            {
                FileSystem::move(oldConfigDirectory() / file, configDirectory() / file);
            }

            if (settings.resourcesPath == oldConfigDirectory() / ResourcesDir)
            {
                settings.resourcesPath = defaultResourcesDir();
                settings.saveTo(ProjectResources::cmsSettingsPath());
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Error during setting up new config directory: " << e.what() << std::endl;
    }
#endif
}

int main()
{
    setupNewConfigDir();

    if (FileSystem::exists(ProjectResources::cmsSettingsPath()))
    {
        boost::process::child playerBin{ProjectResources::playerBinary()};
        playerBin.wait();
    }
    else
    {
        boost::process::child optionsBin{ProjectResources::optionsBinary()};
        optionsBin.wait();
    }
    return 0;
}
