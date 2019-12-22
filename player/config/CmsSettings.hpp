#pragma once

#include "common/NamedField.hpp"
#include "common/types/Uri.hpp"

#include <boost/optional/optional.hpp>

class FilePath;

class CmsSettings
{
public:
    void loadFrom(const FilePath& file);
    void saveTo(const FilePath& file);
    void updateProxy(const std::string& domain, const std::string& username, const std::string& password);

    NamedField<std::string> cmsAddress{"cmsAddress", "http://localhost"};
    NamedField<std::string> key{"key", "yourserverkey"};
    NamedField<std::string> resourcesPath{"localLibrary"};
    NamedField<std::string> displayId{"displayId"};

    std::string domain() const;
    std::string username() const;
    std::string password() const;
    boost::optional<Uri> proxy() const;

private:
    friend class CmsSettingsSerializer;

    NamedField<std::string> domain_{"domain"};
    NamedField<std::string> username_{"username"};
    NamedField<std::string> password_{"password"};
    boost::optional<Uri> proxy_;
};

CmsSettings& cmsSettings();
void updateCmsSettings(const CmsSettings& settings);
