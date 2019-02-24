#include "ImageOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ImageOptions::ImageOptions(int id,
                                         boost::optional<std::string> path,
                                         int duration,
                                         boost::optional<std::string> scaleType,
                                         boost::optional<std::string> align,
                                         boost::optional<std::string> valign) :
    MediaOptions(id, path, duration), m_scaleType(scaleType), m_align(align), m_valign(valign)
{
}

ImageOptions::ImageOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_scaleType = node.get_optional<std::string>(ResourcesXlf::option(ResourcesXlf::Media::Image::ScaleType));
    m_align = node.get_optional<std::string>(ResourcesXlf::option(ResourcesXlf::Media::Image::Align));
    m_valign = node.get_optional<std::string>(ResourcesXlf::option(ResourcesXlf::Media::Image::Valign));
}

boost::optional<std::string> ImageOptions::scaleType() const
{
    return m_scaleType;
}

boost::optional<std::string> ImageOptions::align() const
{
    return m_align;
}

boost::optional<std::string> ImageOptions::valign() const
{
    return m_valign;
}
