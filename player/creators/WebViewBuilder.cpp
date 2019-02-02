#include "WebViewBuilder.hpp"

#include "adaptors/WebKitWebViewAdaptor.hpp"
#include "utils/Resources.hpp"
#include "utils/Logger.hpp"

#include <fstream>
#include <regex>
#include <boost/optional/optional.hpp>

const std::string DEFAULT_EXTENSION = ".html";
const bool DEFAULT_TRANSPARENT = true;
const std::string DEFAULT_LOCAL_SCHEME = "file://";
const std::regex DURATION_REGEX("DURATION=([0-9]+)");

std::unique_ptr<WebView> WebViewBuilder::build()
{
    auto webview = createWebView();
    prepareCommonParams(*webview);
    webview->setTransparent(m_transparent);
    return webview;
}

std::unique_ptr<WebView> WebViewBuilder::createWebView()
{
    return std::make_unique<WebView>(m_id, m_width, m_height, m_path, createAdaptor());
}

std::unique_ptr<IWebViewAdaptor> WebViewBuilder::createAdaptor()
{
    return std::make_unique<WebKitWebViewAdaptor>();
}

WebViewBuilder& WebViewBuilder::path(const boost::optional<std::string>& uri)
{
    if(m_mode == WebViewMode::FileResource)
    {
        auto fileName = std::to_string(m_id) + DEFAULT_EXTENSION;
        m_path += DEFAULT_LOCAL_SCHEME;
        m_path += Resources::directory() / fileName;
    }
    else
    {
        m_path = removeEscapedSymbolsFromUrl(uri.value());
    }
    return *this;
}

std::string WebViewBuilder::removeEscapedSymbolsFromUrl(std::string url)
{
    boost::replace_all(url, "%2F", "/");
    boost::replace_all(url, "%3A", ":");

    return url;
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

WebViewBuilder& WebViewBuilder::modeId(boost::optional<int> modeId)
{
    const int DEFAULT_MODEID = 0;
    const int NATIVE_MODEID = 1;

    int mode = modeId.value_or(DEFAULT_MODEID);
    if(mode == NATIVE_MODEID)
    {
        m_mode = WebViewMode::WebBrowser;
    }
    else
    {
        m_mode = WebViewMode::FileResource;
    }

    return *this;
}

WebViewBuilder& WebViewBuilder::transparent(const boost::optional<bool>& transparent)
{
    m_transparent = transparent.value_or(DEFAULT_TRANSPARENT);
    return *this;
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
