#include "ImageOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::ImageOptions::ImageOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_scaleType = node.get_optional<std::string>(option(Media::Image::ScaleType));
    m_align = node.get_optional<std::string>(option(Media::Image::Align));
    m_valign = node.get_optional<std::string>(option(Media::Image::Valign));
}

boost::optional<std::string> ResourcesXlf::ImageOptions::scaleType() const
{
    return m_scaleType;
}

boost::optional<std::string> ResourcesXlf::ImageOptions::align() const
{
    return m_align;
}

boost::optional<std::string> ResourcesXlf::ImageOptions::valign() const
{
    return m_valign;
}
