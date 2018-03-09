#pragma once

#include "Transition.hpp"
#include "Media.hpp"

#include <gtkmm/fixed.h>
#include <vector>
#include <memory>

class Region : public Gtk::Fixed
{
public:
    Region(uint id,
           const Size& size,
           const Point& pos,
           int zindex,
           bool looped,
           const Transition& transition);

    uint id() const;
    const Size& size() const;
    const Point& position() const;
    int zindex() const;
    bool looped() const;
    const Transition& transition() const;

    template <typename MediaType, typename... Args>
    void add_media(Args... args);
    void show();

private:
    void on_media_timeout();

private:
    uint m_id;
    Size m_size;
    Point m_pos;
    int m_zindex;
    bool m_looped;
    Transition m_transition;

    std::vector<std::unique_ptr<Media>> m_medias;

    size_t m_currentIndex = 0;
    size_t m_previousIndex = 0;

};

template <typename MediaType, typename... Args>
void Region::add_media(Args... args)
{
    m_medias.push_back(std::make_unique<MediaType>(m_size, std::forward<Args>(args)...));
    m_medias.back()->media_timeout().connect(sigc::mem_fun(*this, &Region::on_media_timeout));
    put(m_medias.back()->handler(), 0, 0);
}
