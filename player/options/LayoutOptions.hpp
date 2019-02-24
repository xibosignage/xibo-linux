#pragma once

#include "constants.hpp"
#include <boost/optional/optional.hpp>

class LayoutOptions
{
public:
    LayoutOptions(int width, int height);
    LayoutOptions(const xml_node& node);

    int width() const;
    int height() const;

private:
    int m_width;
    int m_height;
};
