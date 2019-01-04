#pragma once

#include "MediaOptions.hpp"

namespace ResourcesXlf
{
    class WebViewOptions : public MediaOptions
    {
    public:
        WebViewOptions(const xlf_node& node);

        boost::optional<bool> transparent() const;

    private:
        boost::optional<bool> m_transparent;
    };
}

