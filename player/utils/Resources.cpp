#include "Resources.hpp"

std::string ResourcesXlf::attr(const std::string& property)
{
    return Attrs + "." + property;
}

std::string ResourcesXlf::option(const std::string& property)
{
    return Options + "." + property;
}

int ResourcesXlf::Layout::width(const xlf_node& node)
{
    return node.get<int>(attr(Width));
}

int ResourcesXlf::Layout::height(const xlf_node& node)
{
    return node.get<int>(attr(Height));
}

boost::optional<std::string> ResourcesXlf::Layout::backgroundPath(const xlf_node& node)
{
    return node.get_optional<std::string>(attr(BackgroundPath));
}

boost::optional<std::string> ResourcesXlf::Layout::backgroundColor(const xlf_node& node)
{
    return node.get_optional<std::string>(attr(BackgroundColor));
}

int ResourcesXlf::Region::width(const xlf_node& node)
{
    return static_cast<int>(node.get<float>(attr(Width)));
}

int ResourcesXlf::Region::height(const xlf_node& node)
{
    return static_cast<int>(node.get<float>(attr(Height)));
}

int ResourcesXlf::Region::left(const xlf_node& node)
{
    return static_cast<int>(node.get<float>(attr(Left)));
}

int ResourcesXlf::Region::top(const xlf_node& node)
{
    return static_cast<int>(node.get<float>(attr(Top)));
}

boost::optional<int> ResourcesXlf::Region::zindex(const xlf_node& node)
{
    return node.get_optional<int>(attr(Zindex));
}

boost::optional<bool> ResourcesXlf::Region::loop(const xlf_node& node)
{
    return node.get_optional<bool>(option(Loop));
}

std::string ResourcesXlf::Media::type(const xlf_node& node)
{
    return node.get<std::string>(attr(Type));
}

FilePath Resources::directory_;

FilePath Resources::directory()
{
    return directory_;
}

void Resources::setDirectory(const FilePath& directory)
{
    directory_ = directory;
}
