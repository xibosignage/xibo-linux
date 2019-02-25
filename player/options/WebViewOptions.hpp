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

    enum class Mode
    {
        FileResource,
        WebBrowser
    };

    WebViewOptions(int id,
                   boost::optional<std::string> uri,
                   int duration,
                   boost::optional<Transparency> transparency,
                   boost::optional<Mode> mode);
    WebViewOptions(const xml_node& node);

    boost::optional<Transparency> transparency() const;
    boost::optional<Mode> mode() const;

private:
    boost::optional<Transparency> m_transparency;
    boost::optional<Mode> m_mode;

};

std::istream& operator >>(std::istream& in, WebViewOptions::Transparency& transparency);
std::istream& operator >>(std::istream& in, WebViewOptions::Mode& mode);
