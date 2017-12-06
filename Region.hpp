#pragma once

#include "Transition.hpp"
#include "Media.hpp"

#include <gtkmm/fixed.h>
#include <vector>
#include <memory>

class Region : public Gtk::Fixed
{
public:
    Region(int id,
             const Size& size,
             const Point& pos,
             int zindex,
             bool looped,
             const Transition& transition);

    int id() const;
    const Size& size() const;
    const Point& position() const;
    int zindex() const;
    bool looped() const;
    const Transition& transition() const;

    void add_media(const std::shared_ptr<Media>& media);
    // temp
    const std::vector<std::shared_ptr<Media>>& medias() const { return m_medias; }

private:
    int m_id;
    Size m_size;
    Point m_pos;
    int m_zindex;
    bool m_looped;
    Transition m_transition;

    std::vector<std::shared_ptr<Media>> m_medias;

    size_t m_currentIndex = 0;
    size_t m_previousIndex = 0;

};
