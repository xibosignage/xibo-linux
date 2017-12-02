#include "Layout.hpp"

Layout::Layout(int schema_version,
               int width,
               int height,
               const std::string& background_image,
               const std::string& background_color) :
    m_schema_version(schema_version),
    m_width(width),
    m_height(height),
    m_background_image(background_image),
    m_background_color(background_color)
{

}

int Layout::schema_version() const
{
    return m_schema_version;
}

int Layout::width() const
{
    return m_width;
}

int Layout::height() const
{
    return m_height;
}

const std::string& Layout::background_image() const
{
    return m_background_image;
}

const std::string& Layout::background_color() const
{
    return m_background_color;
}

void Layout::add_region(const std::shared_ptr<Region>& region)
{
    m_regions.push_back(region);
}
