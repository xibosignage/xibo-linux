#pragma once

#include "media/Media.hpp"
#include "BaseRegion.hpp"

#include <gtkmm/fixed.h>
#include <vector>
#include <memory>

class Region : public BaseRegion
{
public:
    Region(int id,
           const Size& size,
           const Point& pos,
           int zindex,
           bool looped);

    int id() const;
    const Size& size() const;
    const Point& position() const;
    int zindex() const;
    bool looped() const;
    void show();

    void add_media(std::unique_ptr<Media> media);
    sigc::signal<void> request_handler() const;

private:
    void on_media_timeout();

private:
    int m_id;
    Size m_size;
    Point m_pos;
    int m_zindex;
    bool m_looped;

    std::vector<std::unique_ptr<Media>> m_media;
    sigc::signal<void> m_request_handler;

    size_t m_currentMediaIndex = 0;
};
