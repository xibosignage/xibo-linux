#include "Image.hpp"

Image::Image(int id, int duration, Render render, const std::string& uri, ScaleType scaleType, Align align, Valign valign) :
    Media(id, duration, render, uri), scaleType(scaleType), align(align), valign(valign)
{

}
