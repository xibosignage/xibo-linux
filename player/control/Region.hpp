#pragma once

#include "BaseRegion.hpp"
#include "media/Media.hpp"

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

    static std::unique_ptr<Region> create(int id,
                                          int width,
                                          int height,
                                          int top,
                                          int left,
                                          int zindex,
                                          bool loop);

    int id() const override;
    const Size& size() const override;
    const Point& position() const override;
    void set_size(int width, int height) override;
    int zindex() const override;
    bool looped() const override;
    void show() override;

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
    size_t m_currentMediaIndex = 0;
};
