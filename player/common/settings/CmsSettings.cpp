#include "CmsSettings.hpp"
#include "CmsSettingsSerializer.hpp"

void CmsSettings::loadFrom(const FilePath& settingsFile)
{
    CmsSettingsSerializer serializer;
    serializer.loadFrom(settingsFile, *this);
}

void CmsSettings::saveTo(const FilePath& settingsFile)
{
    CmsSettingsSerializer serializer;
    serializer.saveTo(settingsFile, *this);
}
