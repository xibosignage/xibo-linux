#include "WebViewParser.hpp"

ParsedWebView WebViewParser::parse()
{
    int mode_id = m_options.get_optional<int>("modeId").value_or(-1);
    bool transparency = m_options.get_optional<bool>("transparency").value_or(true);

    return std::make_tuple(m_id, m_duration, m_use_duration, m_uri, mode_id, transparency);
}
