#pragma once

#include "SettingsManager.hpp"
#include "CmsSettings.hpp"

class CmsSettingsManager : public SettingsManager<CmsSettings>
{
public:
    CmsSettingsManager(const FilePath& settingsFile);

protected:
    CmsSettings loadImpl() override;
    void updateImpl(const CmsSettings& settings) override;

};
