#pragma once

#include "Media.hpp"
#include <gtkmm/image.h>

class Image : public Media
{
public:
    enum class ScaleType
    {
        Scaled,
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

    void stop() override;
    void start() override;

private:
    bool is_scaled(ScaleType scale_type);
    int get_left_pos(Align align);
    int get_top_pos(Valign valign);

private:
    Gtk::Image m_handler;
    Size m_size;

};
