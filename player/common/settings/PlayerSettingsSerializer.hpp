#pragma once

#include "common/settings/PlayerSettings.hpp"
#include "common/settings/SettingsSerializer.hpp"

class PlayerSettingsSerializer : public SettingsSerializer<PlayerSettings>
{
public:
    void loadFrom(const FilePath& file, PlayerSettings& settings) override;
    void loadFrom(const XmlNode& node, PlayerSettings& settings);
    void saveTo(const FilePath& file, const PlayerSettings& settings) override;
};
