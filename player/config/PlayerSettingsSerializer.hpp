#pragma once

#include "config/PlayerSettings.hpp"
#include "config/SettingsSerializer.hpp"

class PlayerSettingsSerializer : public SettingsSerializer<PlayerSettings>
{
public:
    void loadSettingsFrom(const FilePath& file, PlayerSettings& settings) override;
    void saveSettingsTo(const FilePath& file, const PlayerSettings& settings) override;

protected:
    XmlDocVersion currentVersion() const override;
};
