#include "PlayerSettings.hpp"
#include "PlayerSettingsSerializer.hpp"

void PlayerSettings::loadFrom(const FilePath& file)
{
    PlayerSettingsSerializer serializer;
    serializer.loadFrom(file, *this);
}

void PlayerSettings::loadFrom(const xml_node& node)
{
    PlayerSettingsSerializer serializer;
    serializer.loadFrom(node, *this);
}

void PlayerSettings::saveTo(const FilePath& file)
{
    PlayerSettingsSerializer serializer;
    serializer.saveTo(file, *this);
}
