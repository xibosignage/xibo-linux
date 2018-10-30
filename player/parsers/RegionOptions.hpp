#pragma once

#include "constants.hpp"
#include <boost/optional/optional.hpp>

namespace ResourcesXlf
{
    class RegionOptions
    {
    public:
        RegionOptions(const xlf_node& node);

        int id() const;
        int width() const;
        int height() const;
        int left() const;
        int top() const;
        boost::optional<int> zindex() const;
        boost::optional<bool> loop() const;

    private:
        int m_id;
        int m_width;
        int m_height;
        int m_left;
        int m_top;
        boost::optional<int> m_zindex;
        boost::optional<bool> m_loop;
    };
}
