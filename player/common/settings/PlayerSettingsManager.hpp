#pragma once

#include "SettingsManager.hpp"
#include "PlayerSettings.hpp"

class PlayerSettingsManager : public SettingsManager<PlayerSettings>
{
public:
    PlayerSettingsManager(const FilePath& settingsFile);

protected:
    PlayerSettings loadImpl() override;
    void updateImpl(const PlayerSettings& settings) override;

};
