#pragma once

#include "PlayerSettings.hpp"
#include "SettingsSerializer.hpp"

class PlayerSettingsSerializer : public SettingsSerializer<PlayerSettings>
{
public:
    void loadFrom(const FilePath& file, PlayerSettings& settings) override;
    void loadFrom(const ptree_node& node, PlayerSettings& settings);
    void saveTo(const FilePath& file, const PlayerSettings& settings) override;

private:
    LoggingLevel toLogLevelEnum(const std::string& level);
};
