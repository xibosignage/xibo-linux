#include "CmsSettings.hpp"

void CmsSettings::loadFrom(const FilePath& settingsFile)
{
    loadFromImpl(settingsFile,
                 m_cmsAddress,
                 m_key,
                 m_resourcesPath,
                 m_username,
                 m_password,
                 m_domain,
                 m_displayId);
}

void CmsSettings::saveTo(const FilePath& settingsFile)
{
    saveToImpl(settingsFile,
               m_cmsAddress,
               m_key,
               m_resourcesPath,
               m_username,
               m_password,
               m_domain,
               m_displayId);
}

std::string CmsSettings::cmsAddress() const
{
    return m_cmsAddress;
}

void CmsSettings::setCmsAddress(const std::string& cmsAddress)
{
    m_cmsAddress = cmsAddress;
}

std::string CmsSettings::key() const
{
    return m_key;
}

void CmsSettings::setKey(const std::string& key)
{
    m_key = key;
}

std::string CmsSettings::resourcesPath() const
{
    return m_resourcesPath;
}

void CmsSettings::setResourcesPath(const std::string& resourcesPath)
{
    m_resourcesPath = resourcesPath;
}

std::string CmsSettings::username() const
{
    return m_username;
}

void CmsSettings::setUsername(const std::string& username)
{
    m_username = username;
}

std::string CmsSettings::password() const
{
    return m_password;
}

void CmsSettings::setPassword(const std::string& password)
{
    m_password = password;
}

std::string CmsSettings::domain() const
{
    return m_domain;
}

void CmsSettings::setDomain(const std::string& domain)
{
    m_domain = domain;
}

std::string CmsSettings::displayId() const
{
    return m_displayId;
}

void CmsSettings::setDisplayId(const std::string& displayId)
{
    m_displayId = displayId;
}
