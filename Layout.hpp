#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "Region.hpp"

#include <vector>
#include <memory>

class Layout
{
public:
    Layout();
    Layout(int schemaVersion,
           int width,
           int height,
           const std::string& backgroundImage,
           const std::string& backgroundColor);

    int schemaVersion;
    int width;
    int height;
    std::string backgroundImage;
    std::string backgroundColor;

    std::vector<std::shared_ptr<Region>> regions;
};

#endif // LAYOUT_HPP
