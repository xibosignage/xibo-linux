#pragma once

#include "Settings.hpp"

class CmsSettings : public Settings
{
public:
    void loadFrom(const FilePath& file) override;
    void saveTo(const FilePath& file) override;

    std::string cmsAddress() const;
    void setCmsAddress(const std::string& value);

    std::string key() const;
    void setKey(const std::string& value);

    std::string resourcesPath() const;
    void setResourcesPath(const std::string& value);

    std::string username() const;
    void setUsername(const std::string& value);

    std::string password() const;
    void setPassword(const std::string& value);

    std::string domain() const;
    void setDomain(const std::string& value);

    std::string displayId() const;
    void setDisplayId(const std::string& value);

private:
    Field<std::string> m_cmsAddress{"cmsAddress", "http://localhost"};
    Field<std::string> m_key{"key", "yourserverkey"};
    Field<std::string> m_resourcesPath{"localLibrary"};
    Field<std::string> m_username{"username"};
    Field<std::string> m_password{"password"};
    Field<std::string> m_domain{"domain"};
    Field<std::string> m_displayId{"displayId"};
};
