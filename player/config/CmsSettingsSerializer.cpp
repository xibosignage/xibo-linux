#include "CmsSettingsSerializer.hpp"

#include "common/logger/Logging.hpp"

void CmsSettingsSerializer::loadFrom(const FilePath& file, CmsSettings& settings)
{
    loadFromImpl(file,
                 settings.address_,
                 settings.key_,
                 settings.resourcesPath_,
                 settings.username_,
                 settings.password_,
                 settings.domain_,
                 settings.displayId_);

    settings.proxy_ = proxyFrom(settings.domain_, settings.username_, settings.password_);
}

void CmsSettingsSerializer::saveTo(const FilePath& file, const CmsSettings& settings)
{
    saveToImpl(file,
               settings.address_,
               settings.key_,
               settings.resourcesPath_,
               settings.username_,
               settings.password_,
               settings.domain_,
               settings.displayId_);
}

boost::optional<Uri> CmsSettingsSerializer::proxyFrom(const std::string& domain,
                                                      const std::string& username,
                                                      const std::string& password)
{
    if (domain.empty()) return {};

    try
    {
        auto domainUri = Uri::fromString(domain);

        boost::optional<Uri::UserInfo> userInfo;
        if (!username.empty())
        {
            userInfo = Uri::UserInfo{username, password};
        }

        return Uri{domainUri.scheme(),
                   Uri::Authority{userInfo, domainUri.authority().host(), domainUri.authority().optionalPort()},
                   domainUri.path()};
    }
    catch (std::exception&)
    {
        Log::error("Proxy invalid");
    }

    return {};
}
