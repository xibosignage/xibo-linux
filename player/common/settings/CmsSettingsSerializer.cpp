#include "CmsSettingsSerializer.hpp"

void CmsSettingsSerializer::loadFrom(const FilePath& file, CmsSettings& settings)
{
    loadFromImpl(file, settings.cmsAddress, settings.key, settings.resourcesPath, settings.username, settings.password,
                 settings.domain, settings.displayId);
}

void CmsSettingsSerializer::saveTo(const FilePath& file, const CmsSettings& settings)
{
    saveToImpl(file, settings.cmsAddress, settings.key, settings.resourcesPath, settings.username, settings.password,
               settings.domain, settings.displayId);
}
