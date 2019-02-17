#pragma once

#include "MediaOptions.hpp"

namespace ResourcesXlf
{
    class WebViewOptions : public MediaOptions
    {
    public:
        WebViewOptions(int id, boost::optional<std::string> path, int duration, bool transparent);
        WebViewOptions(const xml_node& node);

        boost::optional<bool> transparent() const;

    private:
        boost::optional<bool> m_transparent;
    };
}

