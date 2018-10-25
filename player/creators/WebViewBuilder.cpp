#include "WebViewBuilder.hpp"

#include "media/WebView.hpp"
#include "adaptors/WebKitWebViewAdaptor.hpp"
#include "utils/Resources.hpp"

#include <spdlog/spdlog.h>
#include <fstream>
#include <regex>

const std::string DEFAULT_EXTENSION = ".htm";

std::unique_ptr<IMedia> WebViewBuilder::doBuild()
{
    auto webview = std::make_unique<WebView>(m_width, m_height, m_path, std::make_unique<WebKitWebViewAdaptor>());
    webview->setTransparent(m_transparent);
    return webview;
}

WebViewBuilder& WebViewBuilder::path(const boost::optional<std::string>&)
{
    auto filename = std::to_string(m_id) + DEFAULT_EXTENSION;
    m_path = Resources::directory() / filename;
    return *this;
}

WebViewBuilder& WebViewBuilder::duration(int duration)
{
    m_duration = parseDuration(m_path).value_or(duration);
    return *this;
}

WebViewBuilder& WebViewBuilder::width(int width)
{
    m_width = width;
    return *this;
}

WebViewBuilder& WebViewBuilder::height(int height)
{
    m_height = height;
    return *this;
}

WebViewBuilder& WebViewBuilder::transparent(const boost::optional<bool>& transparent)
{
    m_transparent = transparent.value_or(true);
    return *this;
}

boost::optional<int> WebViewBuilder::parseDuration(const FilePath& path)
{
    std::ifstream in(path.string());
    std::string line;
    std::regex re("<!-- DURATION=([0-9]+) -->");
    while(std::getline(in, line))
    {
        std::smatch match;
        if(std::regex_search(line, match, re) && match.size() > 1)
        {
            spdlog::get(LOGGER)->debug("DURATION parsed");
            // NOTE: 0 for full match, 1 for the first group match
            return std::stoi(match[1].str());
        }
    }
    return {};
}
