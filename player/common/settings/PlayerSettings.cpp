#include "PlayerSettings.hpp"

#include "common/logger/Logging.hpp"
#include "common/settings/PlayerSettingsSerializer.hpp"

void PlayerSettings::fromFile(const FilePath& file)
{
    try
    {
        PlayerSettingsSerializer serializer;
        serializer.loadFrom(file, *this);
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
    }
}

void PlayerSettings::fromNode(const XmlNode& node)
{
    try
    {
        PlayerSettingsSerializer serializer;
        serializer.loadFrom(node, *this);
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
    }
}

void PlayerSettings::saveTo(const FilePath& file)
{
    try
    {
        PlayerSettingsSerializer serializer;
        serializer.saveTo(file, *this);
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
    }
}
