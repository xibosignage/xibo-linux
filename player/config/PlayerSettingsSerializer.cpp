#include "PlayerSettingsSerializer.hpp"

const std::string DocumentVersion{"2"};

void PlayerSettingsSerializer::loadSettingsFrom(const FilePath& file, PlayerSettings& settings)
{
    loadFromImpl(loadXmlFrom(file),
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

void PlayerSettingsSerializer::saveSettingsTo(const FilePath& file, const PlayerSettings& settings)
{
    auto tree = saveToImpl(settings.size_,
                           settings.position_,
                           settings.logLevel_,
                           settings.displayName_,
                           settings.preventSleep_,
                           settings.statsEnabled_,
                           settings.collectInterval_,
                           settings.xmrNetworkAddress_,
                           settings.embeddedServerPort_,
                           settings.screenshotInterval_);
    saveXmlTo(file, tree);
}

XmlDefaultFileLoader::DocVersionType PlayerSettingsSerializer::currentVersion() const
{
    return DocumentVersion;
}
