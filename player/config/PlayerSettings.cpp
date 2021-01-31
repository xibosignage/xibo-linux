#include "PlayerSettings.hpp"

#include "common/logger/Logging.hpp"
#include "config/PlayerSettingsSerializer.hpp"

void PlayerSettings::fromFields(const PlayerSettings& settings)
{
    collectInterval_.setValue(settings.collectInterval());
    statsEnabled_.setValue(settings.statsEnabled());
    xmrNetworkAddress_.setValue(settings.xmrNetworkAddress());
    size_.setValue(settings.size().values());
    position_.setValue(settings.position().values());
    logLevel_.setValue(settings.logLevel());
    screenshotInterval_.setValue(settings.screenshotInterval());
    embeddedServerPort_.setValue(settings.embeddedServerPort());
    preventSleep_.setValue(settings.preventSleep());
    displayName_.setValue(settings.displayName());
}

void PlayerSettings::fromFile(const FilePath& file)
{
    try
    {
        PlayerSettingsSerializer serializer;
        serializer.loadSettingsFrom(file, *this);
    }
    catch (PlayerRuntimeError& e)
    {
        Log::error("[PlayerSettings] Load error: {}", e.message());
    }
    catch (std::exception& e)
    {
        Log::error("[PlayerSettings] Load error: {}", e.what());
    }
}

void PlayerSettings::saveTo(const FilePath& file)
{
    try
    {
        PlayerSettingsSerializer serializer;
        serializer.saveSettingsTo(file, *this);
    }
    catch (PlayerRuntimeError& e)
    {
        Log::error("[PlayerSettings] Save error: {}", e.message());
    }
    catch (std::exception& e)
    {
        Log::error("[PlayerSettings] Save error: {}", e.what());
    }
}

Field<int>& PlayerSettings::collectInterval()
{
    return collectInterval_;
}

const Field<int>& PlayerSettings::collectInterval() const
{
    return collectInterval_;
}

Field<bool>& PlayerSettings::statsEnabled()
{
    return statsEnabled_;
}

const Field<bool>& PlayerSettings::statsEnabled() const
{
    return statsEnabled_;
}

Field<std::string>& PlayerSettings::xmrNetworkAddress()
{
    return xmrNetworkAddress_;
}

const Field<std::string>& PlayerSettings::xmrNetworkAddress() const
{
    return xmrNetworkAddress_;
}

Field<std::string>& PlayerSettings::logLevel()
{
    return logLevel_;
}

const Field<std::string>& PlayerSettings::logLevel() const
{
    return logLevel_;
}

Field<int>& PlayerSettings::screenshotInterval()
{
    return screenshotInterval_;
}

const Field<int>& PlayerSettings::screenshotInterval() const
{
    return screenshotInterval_;
}

Field<unsigned short>& PlayerSettings::embeddedServerPort()
{
    return embeddedServerPort_;
}

const Field<unsigned short>& PlayerSettings::embeddedServerPort() const
{
    return embeddedServerPort_;
}

Field<bool>& PlayerSettings::preventSleep()
{
    return preventSleep_;
}

const Field<bool>& PlayerSettings::preventSleep() const
{
    return preventSleep_;
}

Field<std::string>& PlayerSettings::displayName()
{
    return displayName_;
}

const Field<std::string>& PlayerSettings::displayName() const
{
    return displayName_;
}

PlayerSettings::SizeField& PlayerSettings::size()
{
    return size_;
}

const PlayerSettings::SizeField& PlayerSettings::size() const
{
    return size_;
}

PlayerSettings::PositionField& PlayerSettings::position()
{
    return position_;
}

const PlayerSettings::PositionField& PlayerSettings::position() const
{
    return position_;
}
