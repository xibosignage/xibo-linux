#pragma once

#include "common/NamedField.hpp"
#include "common/fs/FilePath.hpp"
#include "common/types/Uri.hpp"

#include <boost/optional/optional.hpp>

class FilePath;

class CmsSettings
{
    friend class CmsSettingsSerializer;

public:
    void fromFile(const FilePath& file);
    void saveTo(const FilePath& file);
    void updateProxy(const std::string& domain_, const std::string& username_, const std::string& password_);

    boost::optional<Uri> proxy() const;

    Field<std::string>& address();
    const Field<std::string>& address() const;

    Field<std::string>& key();
    const Field<std::string>& key() const;

    Field<FilePath>& resourcesPath();
    const Field<FilePath>& resourcesPath() const;

    Field<std::string>& displayId();
    const Field<std::string>& displayId() const;

    const Field<std::string>& domain() const;
    const Field<std::string>& username() const;
    const Field<std::string>& password() const;

private:
    NamedField<std::string> address_{"cmsAddress", "http://localhost"};
    NamedField<std::string> key_{"key", "yourserverkey"};
    NamedField<FilePath> resourcesPath_{"localLibrary"};
    NamedField<std::string> displayId_{"displayId"};
    NamedField<std::string> domain_{"domain"};
    NamedField<std::string> username_{"username"};
    NamedField<std::string> password_{"password"};
    boost::optional<Uri> proxy_;
};
