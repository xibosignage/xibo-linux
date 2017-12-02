#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Media.hpp"

class Image : public Media
{
public:

    enum class ScaleType
    {
        Center,
        Stretch,
        Invalid
    };

    enum class Align
    {
        Left,
        Center,
        Right,
        Invalid
    };

    enum class Valign
    {
        Top,
        Middle,
        Bottom,
        Invalid
    };

    Image(int id, int duration, Render render, const std::string& uri, ScaleType scale_type, Align align, Valign valign);

    ScaleType scale_type() const;
    Align align() const;
    Valign valign() const;

private:
    ScaleType m_scale_type;
    Align m_align;
    Valign m_valign;

};


#endif // IMAGE_HPP
