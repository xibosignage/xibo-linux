#include "WebViewBuilder.hpp"

#include "utils/Resources.hpp"
#include "utils/Logger.hpp"
#include "adaptors/WebKitWebViewAdaptor.hpp"

#include <fstream>
#include <regex>
#include <boost/optional/optional.hpp>

const std::string DEFAULT_NATIVE_SCHEME = "file://";
const std::regex DURATION_REGEX("DURATION=([0-9]+)");

std::unique_ptr<WebView> WebViewBuilder::create()
{
    return std::unique_ptr<WebView>(new WebView{m_id, m_width, m_height, m_uri, createHandler()});
}

std::unique_ptr<IWebViewAdaptor> WebViewBuilder::createDefaultHandler()
{
    return std::make_unique<WebKitWebViewAdaptor>();
}

void WebViewBuilder::doMediaSetup(WebView& webview)
{
    webview.setTransparent(m_transparency);
}

Uri WebViewBuilder::getUriOption(const boost::optional<std::string>& uriOpt)
{
    if(getModeOption(m_options->mode()) == WebViewOptions::Mode::FileResource)
    {
        auto fileName = std::to_string(m_id) + DEFAULT_WEBVIEW_EXTENSION;
        return Uri(DEFAULT_NATIVE_SCHEME + (Resources::directory() / fileName).string());
    }
    else
    {
        return Uri{removeEscapedSymbolsFromUri(uriOpt.value())};
    }
}

void WebViewBuilder::retrieveMediaOptions(const WebViewOptions& opts)
{
    m_transparency = getTransparentOption(opts.transparency());
    m_mode = getModeOption(opts.mode());
}

std::string WebViewBuilder::removeEscapedSymbolsFromUri(std::string url)
{
    boost::replace_all(url, "%2F", "/");
    boost::replace_all(url, "%3A", ":");

    return url;
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

int WebViewBuilder::getDurationOption(int duration)
{
    return parseDuration(m_uri.path()).value_or(duration);
}

WebViewOptions::Transparency WebViewBuilder::getTransparentOption(const boost::optional<WebViewOptions::Transparency>& transparentOpt)
{
    return transparentOpt.value_or(DEFAULT_TRANSPARENCY);
}

WebViewOptions::Mode WebViewBuilder::getModeOption(const boost::optional<WebViewOptions::Mode>& modeOpt)
{
    return modeOpt.value_or(DEFAULT_WEBVIEW_MODE);
}

boost::optional<int> WebViewBuilder::parseDuration(const FilePath& path)
{
    std::ifstream in(path.string());
    std::istream_iterator<std::string> fileEndIt{};

    std::smatch matchedGroups;
    const int DURATION_GROUP = 1;

    auto it = std::find_if(std::istream_iterator<std::string>{in}, fileEndIt, [=, &matchedGroups](const auto& str){
        return std::regex_search(str, matchedGroups, DURATION_REGEX) && matchedGroups.size() > 1;
    });

    return it != fileEndIt ? std::stoi(matchedGroups[DURATION_GROUP].str()) : boost::optional<int>{};
}
