#pragma once

#include "CmsSettings.hpp"
#include "SettingsSerializer.hpp"

class CmsSettingsSerializer : public SettingsSerializer<CmsSettings>
{
public:
    void loadFrom(const FilePath& file, CmsSettings& settings) override;
    void saveTo(const FilePath& file, const CmsSettings& settings) override;
};
