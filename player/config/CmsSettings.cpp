#include "CmsSettings.hpp"

#include "common/logger/Logging.hpp"
#include "config/CmsSettingsSerializer.hpp"

void CmsSettings::fromFile(const FilePath& settingsFile)
{
    try
    {
        CmsSettingsSerializer serializer;
        serializer.loadSettingsFrom(settingsFile, *this);
    }
    catch (PlayerRuntimeError& e)
    {
        Log::error("[CmsSettings] Load error: {}", e.message());
    }
    catch (std::exception& e)
    {
        Log::error("[CmsSettings] Load error: {}", e.what());
    }
}

void CmsSettings::saveTo(const FilePath& settingsFile)
{
    try
    {
        CmsSettingsSerializer serializer;
        serializer.saveSettingsTo(settingsFile, *this);
    }
    catch (PlayerRuntimeError& e)
    {
        Log::error("[CmsSettings] Load error: {}", e.message());
    }
    catch (std::exception& e)
    {
        Log::error("[CmsSettings] Load error: {}", e.what());
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

boost::optional<Uri> CmsSettings::proxy() const
{
    return proxy_;
}

Field<std::string>& CmsSettings::address()
{
    return address_;
}

const Field<std::string>& CmsSettings::address() const
{
    return address_;
}

Field<std::string>& CmsSettings::key()
{
    return key_;
}

const Field<std::string>& CmsSettings::key() const
{
    return key_;
}

Field<FilePath>& CmsSettings::resourcesPath()
{
    return resourcesPath_;
}

const Field<FilePath>& CmsSettings::resourcesPath() const
{
    return resourcesPath_;
}

Field<std::string>& CmsSettings::displayId()
{
    return displayId_;
}

const Field<std::string>& CmsSettings::displayId() const
{
    return displayId_;
}

const Field<std::string>& CmsSettings::domain() const
{
    return domain_;
}

const Field<std::string>& CmsSettings::username() const
{
    return username_;
}

const Field<std::string>& CmsSettings::password() const
{
    return password_;
}
