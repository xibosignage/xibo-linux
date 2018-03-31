#pragma once

#include "Media.hpp"
#include <gtkmm/image.h>

class Image : public Media
{
public:
    Image(const Size& size, int id, int duration, const std::string& uri,
          const std::string& scale_type, const std::string& align, const std::string& valign);

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

    ScaleType scale_type() const;
    Align align() const;
    Valign valign() const;

    Gtk::Widget& handler() override;
    void hide() override;
    void show() override;

private:
    ScaleType to_scale_type(const std::string& scale_type);
    Align to_align(const std::string& align);
    Valign to_valign(const std::string& valign);

private:
    ScaleType m_scale_type;
    Align m_align;
    Valign m_valign;
    Gtk::Image m_handler;

};
