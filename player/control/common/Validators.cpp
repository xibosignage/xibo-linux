#include "Validators.hpp"

#include "utils/Resources.hpp"
#include "utils/FileSystemAdaptor.hpp"
#include "utils/ColorToHexConverter.hpp"

Uri Validators::validateUri(const boost::optional<std::string>& uri)
{
    if(uri)
    {
        auto filesystem = std::make_unique<FileSystemAdaptor>();
        auto fullPath = Resources::directory() / uri.value();

        if(!filesystem->isRegularFile(fullPath))
            throw std::runtime_error("Not valid path");

        return Uri{Uri::Scheme::File, fullPath};
    }
    return {};
}

uint32_t Validators::validateColor(const std::string& color)
{
    ColorToHexConverter converter;
    return converter.colorToHex(color);
}
