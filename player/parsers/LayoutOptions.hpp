#pragma once

#include "constants.hpp"
#include <boost/optional/optional.hpp>

namespace ResourcesXlf
{
    class LayoutOptions
    {
    public:
        LayoutOptions(const xlf_node& node);

        int width() const;
        int height() const;
        boost::optional<std::string> backgroundPath() const;
        boost::optional<std::string> backgroundColor() const;

    private:
        int m_width;
        int m_height;
        boost::optional<std::string> m_backgroundPath;
        boost::optional<std::string> m_backgroundColor;
    };
}
