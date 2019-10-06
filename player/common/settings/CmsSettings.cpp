#include "CmsSettings.hpp"

#include "common/logger/Logging.hpp"
#include "common/settings/CmsSettingsSerializer.hpp"

void CmsSettings::loadFrom(const FilePath& settingsFile)
{
    try
    {
        CmsSettingsSerializer serializer;
        serializer.loadFrom(settingsFile, *this);
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
    }
}

void CmsSettings::saveTo(const FilePath& settingsFile)
{
    try
    {
        CmsSettingsSerializer serializer;
        serializer.saveTo(settingsFile, *this);
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
    }
}
