#pragma once

#include "Media.hpp"
#include <gtkmm/image.h>

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

    Image(const Region& region, int id, int duration, const std::string& uri,
          ScaleType scale_type, Align align, Valign valign);

    ScaleType scale_type() const;
    Align align() const;
    Valign valign() const;

    void stop() override;
    void start() override;

private:
    ScaleType m_scale_type;
    Align m_align;
    Valign m_valign;
    Gtk::Image m_handler;

};
