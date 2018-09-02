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

    Image(int id, int width, int height, int duration, const std::string& uri,
          ScaleType scaleType, Align align, Valign valign);

    void stop() override;
    void start() override;
    void setSize(int width, int height) override;
    void requestHandler() override;

    ScaleType scaleType() const;
    Align align() const;
    Valign valign() const;

private:
    bool isScaled() const;
    int getLeftPos() const;
    int getTopPos() const;

private:
    Gtk::Image m_handler;
    int m_actualWidth;
    int m_actualHeight;
    ScaleType m_scaleType;
    Align m_align;
    Valign m_valign;

};
