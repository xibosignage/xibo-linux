#include "WebViewBuilder.hpp"

#include "utils/Resources.hpp"
#include "utils/Logger.hpp"
#include "adaptors/WebKitWebViewAdaptor.hpp"

#include <fstream>
#include <regex>
#include <boost/optional/optional.hpp>

const std::string DEFAULT_EXTENSION = ".html";

std::unique_ptr<WebView> WebViewBuilder::create()
{
    return std::unique_ptr<WebView>(new WebView{m_id, m_width, m_height, m_path, createHandler()});
}

std::unique_ptr<IWebViewAdaptor> WebViewBuilder::createDefaultHandler()
{
    return std::make_unique<WebKitWebViewAdaptor>();
}

void WebViewBuilder::doMediaSetup(WebView& webview)
{
    webview.setTransparent(m_transparency);
}

WebViewBuilder& WebViewBuilder::retrieveMediaOptions(const WebViewOptions& opts)
{
    m_transparency = getTransparentOption(opts.transparency());
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


FilePath WebViewBuilder::getPathOption(const boost::optional<std::string>&)
{
    auto filename = std::to_string(m_id) + DEFAULT_EXTENSION;
    return Resources::directory() / filename;
}

int WebViewBuilder::getDurationOption(int duration)
{
    return parseDuration(m_path).value_or(duration);
}

WebViewOptions::Transparency WebViewBuilder::getTransparentOption(const boost::optional<WebViewOptions::Transparency>& transparentOpt)
{
    return transparentOpt.value_or(DEFAULT_TRANSPARENCY);
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
            Log::debug("DURATION parsed from .html {}", match[1].str());
            // NOTE: 0 for full match, 1 for the first group match
            return std::stoi(match[1].str());
        }
    }
    return {};
}
