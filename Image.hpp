#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Media.hpp"
#include "utilities.hpp"

class Image : public Media
{
public:
    Image() = default;

    ScaleType scaleType;
    Align align;
    Valign valign;

};


#endif // IMAGE_HPP
