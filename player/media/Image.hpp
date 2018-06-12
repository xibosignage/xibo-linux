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

    Image(int id, const Size& size, int duration, const std::string& uri,
          ScaleType scale_type, Align align, Valign valign);

    void stop() override;
    void start() override;
    void set_size(int width, int height) override;
    void request_handler() override;

    ScaleType scale_type() const;
    Align align() const;
    Valign valign() const;

private:
    bool is_scaled() const;
    int get_left_pos() const;
    int get_top_pos() const;

private:
    Gtk::Image m_handler;
    Size m_actual_size;
    ScaleType m_scale_type;
    Align m_align;
    Valign m_valign;

};
