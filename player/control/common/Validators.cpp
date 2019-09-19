#include "Validators.hpp"

#include "common/fs/Resources.hpp"
#include "common/fs/FileSystem.hpp"

Uri Validators::validateUri(const boost::optional<std::string>& uri)
{
    if(uri)
    {
        auto fullPath = Resources::directory() / uri.value();

        if(!FileSystem::isRegularFile(fullPath))
            return Uri{uri.value()};

        return Uri{Uri::Scheme::FILE, fullPath.string()};
    }
    return {};
}
