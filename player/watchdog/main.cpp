#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "config/AppConfig.hpp"
#include "config/CmsSettings.hpp"

#include "ProcessWatcher.hpp"

#include <boost/program_options.hpp>

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
int main(int argc, char** argv)
{
    setupNewConfigDir();

    try
    {
        boost::program_options::options_description desc{"Options"};
        desc.add_options()("disable-restart", "Don't restart player (disable watchdog)");
        desc.add_options()("config-app", "Run config application");

        boost::program_options::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);

#if defined(SNAP_ENABLED)
        std::cout << "Running in SNAP environment" << std::endl;
#endif

        if (FileSystem::exists(AppConfig::cmsSettingsPath()) && vm.count("config-app") == 0)
        {
            ProcessWatcher playerWatcher{AppConfig::playerBinary(), vm.count("disable-restart") > 0};
            playerWatcher.run();
        }
        else
        {
            boost::process::child optionsBin{AppConfig::optionsBinary()};
            optionsBin.wait();
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
