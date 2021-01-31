#pragma once

#include "config/CmsSettings.hpp"
#include "config/SettingsSerializer.hpp"

class CmsSettingsSerializer : public SettingsSerializer<CmsSettings>
{
public:
    void loadSettingsFrom(const FilePath& file, CmsSettings& settings) override;
    void saveSettingsTo(const FilePath& file, const CmsSettings& settings) override;

    boost::optional<Uri> proxyFrom(const std::string& domain, const std::string& username, const std::string& password);

protected:
    XmlDocVersion currentVersion() const override;
};
