#include <boost/process/child.hpp>
#include <iostream>

#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "config/AppConfig.hpp"
#include "config/CmsSettings.hpp"

void setupNewConfigDir()
{
#ifdef SNAP_ENABLED
    try
    {
        const std::string CmsSettingsFile = "cmsSettings.xml";
        const std::string PlayerSettingsFile = "playerSettings.xml";
        const std::string PrivateKeyFile = "id_rsa";
        const std::string PublicKeyFile = "id_rsa.pub";
        const std::string ResourcesDir = "resources";
        if (FileSystem::exists(AppConfig::oldConfigDirectory() / CmsSettingsFile))
        {
            CmsSettings settings;
            settings.fromFile(AppConfig::oldConfigDirectory() / CmsSettingsFile);

            std::vector<std::string> filesToMove{CmsSettingsFile, PlayerSettingsFile, PrivateKeyFile, PublicKeyFile};
            for (auto&& file : filesToMove)
            {
                FileSystem::move(AppConfig::oldConfigDirectory() / file, AppConfig::configDirectory() / file);
            }

            if (settings.resourcesPath() == AppConfig::oldConfigDirectory() / ResourcesDir)
            {
                settings.resourcesPath().setValue(AppConfig::configDirectory() / ResourcesDir);
                settings.saveTo(AppConfig::cmsSettingsPath());
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

    if (FileSystem::exists(AppConfig::cmsSettingsPath()))
    {
        while (true)
        {
            boost::process::child playerBin{AppConfig::playerBinary()};
            playerBin.wait();
            std::cerr << "Player exited with code " << playerBin.exit_code() << std::endl;
            sleep(3);
        }
    }
    else
    {
        boost::process::child optionsBin{AppConfig::optionsBinary()};
        optionsBin.wait();
    }
    return 0;
}
