#pragma once

#include "common/settings/CmsSettings.hpp"
#include "common/settings/SettingsSerializer.hpp"

class CmsSettingsSerializer : public SettingsSerializer<CmsSettings>
{
public:
    void loadFrom(const FilePath& file, CmsSettings& settings) override;
    void saveTo(const FilePath& file, const CmsSettings& settings) override;

    boost::optional<Uri> proxyFrom(const std::string& domain, const std::string& username, const std::string& password);
};
