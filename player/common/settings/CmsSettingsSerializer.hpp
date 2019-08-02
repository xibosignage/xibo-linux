#pragma once

#include "SettingsSerializer.hpp"
#include "CmsSettings.hpp"

class CmsSettingsSerializer : public SettingsSerializer<CmsSettings>
{
public:
    void loadFrom(const FilePath& file, CmsSettings& settings) override;
    void saveTo(const FilePath& file, const CmsSettings& settings) override;
};
