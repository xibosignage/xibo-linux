#pragma once

#include "constants.hpp"
#include <boost/optional/optional.hpp>

namespace ResourcesXlf
{
    class BackgroundOptions
    {
    public:
        BackgroundOptions(int width, int height, boost::optional<std::string> path, boost::optional<std::string> color);
        BackgroundOptions(const xml_node& node);

        int width() const;
        int height() const;
        boost::optional<std::string> path() const;
        boost::optional<std::string> color() const;

    private:
        int m_schemaVersion;
        int m_width;
        int m_height;
        boost::optional<std::string> m_path;
        boost::optional<std::string> m_color;
    };
}
