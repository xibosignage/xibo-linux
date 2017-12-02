#include "Image.hpp"

Image::Image(int id, int duration, Render render, const std::string& uri, ScaleType scale_type, Align align, Valign valign) :
    Media(id, duration, render, uri), m_scale_type(scale_type), m_align(align), m_valign(valign)
{

}

Image::ScaleType Image::scale_type() const
{
    return m_scale_type;
}

Image::Align Image::align() const
{
    return m_align;
}

Image::Valign Image::valign() const
{
    return m_valign;
}
