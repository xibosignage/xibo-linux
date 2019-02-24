#pragma once

#include "constants.hpp"
#include <boost/optional/optional.hpp>

class RegionOptions
{
public:
    enum class Loop
    {
        Disable,
        Enable
    };

    RegionOptions(int id, int width, int height, int left, int top, boost::optional<int> zorder, boost::optional<Loop> loop);
    RegionOptions(const xml_node& node);

    int id() const;
    int width() const;
    int height() const;
    int left() const;
    int top() const;
    boost::optional<int> zorder() const;
    boost::optional<Loop> loop() const;

private:
    int m_id;
    int m_width;
    int m_height;
    int m_left;
    int m_top;
    boost::optional<int> m_zindex;
    boost::optional<Loop> m_loop;
};

std::istream& operator >>(std::istream& in, RegionOptions::Loop& loop);
