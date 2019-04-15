#include "CmsSettingsManager.hpp"

CmsSettingsManager::CmsSettingsManager(const FilePath& settingsFile) :
    SettingsManager{settingsFile}
{
}

CmsSettings CmsSettingsManager::loadImpl()
{
    CmsSettings settings;

    loadHelper(settings.cmsAddress,
               settings.key,
               settings.resourcesPath,
               settings.username,
               settings.password,
               settings.domain,
               settings.displayId);

    return settings;
}

void CmsSettingsManager::updateImpl(const CmsSettings& settings)
{
    updateHelper(settings.cmsAddress,
                 settings.key,
                 settings.resourcesPath,
                 settings.username,
                 settings.password,
                 settings.domain,
                 settings.displayId);
}
