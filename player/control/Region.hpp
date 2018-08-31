#pragma once

#include "IRegion.hpp"

#include <vector>
#include <sigc++/signal.h>
#include <gtkmm/fixed.h>

class Region : public IRegion
{
public:
    Region(int id, int width, int height, int left, int top, int zindex, bool looped);
    ~Region() override;

    Region(const Region& other) = delete;
    Region& operator=(const Region& other) = delete;

    int width() const override;
    int height() const override;
    void set_size(int width, int height) override;

    int left() const override;
    int top() const override;
    void set_pos(int left, int top) override;

    int id() const override;
    int zindex() const override;
    bool looped() const override;
    void show() override;

    void add_media(std::unique_ptr<IMedia> media) override;
    Gtk::Fixed& handler() override;

private:
    void on_media_timeout();

private:
    Gtk::Fixed m_handler;

    int m_id;
    int m_width;
    int m_height;
    int m_left;
    int m_top;
    int m_zindex;
    bool m_looped;

    std::vector<std::unique_ptr<IMedia>> m_media;
    size_t m_currentMediaIndex = 0;
};
