#pragma once

#include "Media.hpp"

#include <gtkmm/fixed.h>
#include <memory>
#include <iostream>

class MainLayout;

class MyRegion : public Gtk::Fixed
{
public:
    MyRegion(MainLayout* layout,
           const Point& pos,
           const Size& size,
           int duration,
           int zindex);

    void add_media(const std::shared_ptr<Media>& media);
    void add_media(std::initializer_list<std::shared_ptr<Media>> medias);

    Point position() const;
    Size size() const;

private:
    std::vector<std::shared_ptr<Media>> m_medias;

    Point m_pos;
    Size m_size;
    int m_duration;
    int m_zindex;

    size_t m_currentIndex = 0;
    size_t m_previousIndex = 0;

};
