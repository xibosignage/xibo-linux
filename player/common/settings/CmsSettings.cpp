#include "CmsSettings.hpp"

#include "common/logger/Logging.hpp"
#include "common/settings/CmsSettingsSerializer.hpp"

void CmsSettings::loadFrom(const FilePath& settingsFile)
{
    try
    {
        CmsSettingsSerializer serializer;
        serializer.loadFrom(settingsFile, *this);
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
    }
}

void CmsSettings::saveTo(const FilePath& settingsFile)
{
    try
    {
        CmsSettingsSerializer serializer;
        serializer.saveTo(settingsFile, *this);
    }
    catch (std::exception& e)
    {
        Log::error(e.what());
    }
}

void CmsSettings::updateProxy(const std::string& domain, const std::string& username, const std::string& password)
{
    CmsSettingsSerializer serializer;
    domain_ = domain;
    username_ = username;
    password_ = password;
    proxy_ = serializer.proxyFrom(domain, username, password);
}

std::string CmsSettings::domain() const
{
    return domain_;
}

std::string CmsSettings::username() const
{
    return username_;
}

std::string CmsSettings::password() const
{
    return password_;
}

boost::optional<Uri> CmsSettings::proxy() const
{
    return proxy_;
}
