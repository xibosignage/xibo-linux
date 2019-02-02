#pragma once

#include "MediaOptions.hpp"

namespace ResourcesXlf
{
    class WebViewOptions : public MediaOptions
    {
    public:
        WebViewOptions(const xml_node& node);

        boost::optional<bool> transparent() const;
        boost::optional<int> modeId() const;

    private:
        boost::optional<bool> m_transparent;
        boost::optional<int> m_modeId;
    };
}

