#pragma once

#include "common/Field.hpp"
#include "common/types/Uri.hpp"

#include <boost/optional/optional.hpp>

class FilePath;

class CmsSettings
{
public:
    void loadFrom(const FilePath& file);
    void saveTo(const FilePath& file);
    void updateProxy(const std::string& domain, const std::string& username, const std::string& password);

    Field<std::string> cmsAddress{"cmsAddress", "http://localhost"};
    Field<std::string> key{"key", "yourserverkey"};
    Field<std::string> resourcesPath{"localLibrary"};
    Field<std::string> displayId{"displayId"};

    std::string domain() const;
    std::string username() const;
    std::string password() const;
    boost::optional<Uri> proxy() const;

private:
    friend class CmsSettingsSerializer;

    Field<std::string> domain_{"domain"};
    Field<std::string> username_{"username"};
    Field<std::string> password_{"password"};
    boost::optional<Uri> proxy_;
};

CmsSettings& cmsSettings();
void updateCmsSettings(const CmsSettings& settings);
