#include "Validators.hpp"

#include "utils/Resources.hpp"
#include "utils/ColorToHexConverter.hpp"
#include "common/FileSystem.hpp"

Uri Validators::validateUri(const boost::optional<std::string>& uri)
{
    if(uri)
    {
        auto filesystem = std::make_unique<FileSystem>();
        auto fullPath = Resources::directory() / uri.value();

        if(!FileSystem::isRegularFile(fullPath))
            return Uri{uri.value()};

        return Uri{Uri::Scheme::File, fullPath};
    }
    return {};
}

uint32_t Validators::validateColor(const std::string& color)
{
    ColorToHexConverter converter;
    return converter.colorToHex(color);
}
