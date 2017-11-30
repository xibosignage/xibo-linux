#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Media.hpp"

class Image : public Media
{
public:
    Image(int id, int duration, Render render, const std::string& uri, ScaleType scaleType, Align align, Valign valign);

    ScaleType scaleType;
    Align align;
    Valign valign;

};


#endif // IMAGE_HPP
