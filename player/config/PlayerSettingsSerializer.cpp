#include "PlayerSettingsSerializer.hpp"

void PlayerSettingsSerializer::loadFrom(const FilePath& file, PlayerSettings& settings)
{
    loadFromImpl(file,
                 settings.size_,
                 settings.position_,
                 settings.logLevel_,
                 settings.displayName_,
                 settings.preventSleep_,
                 settings.statsEnabled_,
                 settings.collectInterval_,
                 settings.xmrNetworkAddress_,
                 settings.embeddedServerPort_,
                 settings.screenshotInterval_);
}

void PlayerSettingsSerializer::saveTo(const FilePath& file, const PlayerSettings& settings)
{
    saveToImpl(file,
               settings.size_,
               settings.position_,
               settings.logLevel_,
               settings.displayName_,
               settings.preventSleep_,
               settings.statsEnabled_,
               settings.collectInterval_,
               settings.xmrNetworkAddress_,
               settings.embeddedServerPort_,
               settings.screenshotInterval_);
}
