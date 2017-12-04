#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "Region.hpp"

#include <vector>
#include <memory>

class Layout
{
public:
    Layout(int schema_version,
           int width,
           int height,
           const std::string& background_image,
           const std::string& background_color);

    int schema_version() const;
    int width() const;
    int height() const;
    const std::string& background_image() const;
    const std::string& background_color() const;

    void add_region(const std::shared_ptr<Region>& region);

    // temp
    const std::vector<std::shared_ptr<Region>>& regions() const { return m_regions; }

private:
    int m_schema_version;
    int m_width;
    int m_height;
    std::string m_background_image;
    std::string m_background_color;

    std::vector<std::shared_ptr<Region>> m_regions;

};

#endif // LAYOUT_HPP
