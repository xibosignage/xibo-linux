#ifndef REGION_HPP
#define REGION_HPP

#include "Transition.hpp"
#include "Media.hpp"

#include <vector>
#include <memory>

class Region
{
public:
    Region(int id, int width, int height, int top, int left, int zindex, bool loop, const Transition& transition);

    int id() const;
    int width() const;
    int height() const;
    int top() const;
    int left() const;
    int zindex() const;
    bool loop() const;
    const Transition& transition() const;

    void add_media(const std::shared_ptr<Media>& media);

    // temp
    std::vector<std::shared_ptr<Media>> medias() const { return m_medias; }

private:
    int m_id;
    int m_width;
    int m_height;
    int m_top;
    int m_left;
    int m_zindex;

    bool m_loop;
    Transition m_transition;

    std::vector<std::shared_ptr<Media>> m_medias;
};

#endif // REGION_HPP
