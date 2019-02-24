#pragma once

#include "MediaOptions.hpp"

class WebViewOptions : public MediaOptions
{
public:
    enum class Transparency
    {
        Disable,
        Enable
    };

    WebViewOptions(int id, boost::optional<std::string> path, int duration, boost::optional<Transparency> transparency);
    WebViewOptions(const xml_node& node);

    boost::optional<Transparency> transparency() const;

private:
    boost::optional<Transparency> m_transparency;

};

std::istream& operator >>(std::istream& in, WebViewOptions::Transparency& transparency);
