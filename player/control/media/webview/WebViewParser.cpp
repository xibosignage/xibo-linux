#include "WebViewParser.hpp"

#include "utils/Resources.hpp"
#include "control/media/creators/MediaResources.hpp"
#include "control/media/webview/WebViewResources.hpp"

#include <regex>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>

const bool DEFAULT_TRANSPARENCY = true;
const int DEFAULT_WEBVIEW_MODE = 0;
const int NATIVE_MODEID = 1;

const std::string DEFAULT_NATIVE_SCHEME = "file://";
const std::regex DURATION_REGEX("DURATION=([0-9]+)");
const std::string DEFAULT_WEBVIEW_EXTENSION = ".html";

int WebViewParser::duration()
{
    auto baseDuration = MediaParser::duration();
    return parseDuration(uri().path()).value_or(baseDuration);
}

ExtraOptions WebViewParser::parseAdditonalOptions(const xml_node& node)
{
    auto transparency = node.get<bool>(ResourcesXlf::option(ResourcesXlf::WebView::Transparency), DEFAULT_TRANSPARENCY);
    auto mode = node.get<int>(ResourcesXlf::option(ResourcesXlf::WebView::ModeId), DEFAULT_WEBVIEW_MODE);

    return {
        {ResourcesXlf::WebView::Transparency, std::to_string(transparency)},
        {ResourcesXlf::WebView::ModeId, std::to_string(mode)}
    };
}

std::optional<int> WebViewParser::parseDuration(const FilePath& path)
{
    std::ifstream in(path.string());

    std::smatch matchedGroups;
    std::string line;
    const int DURATION_GROUP = 1;

    while(std::getline(in, line))
    {
        if(std::regex_search(line, matchedGroups, DURATION_REGEX) && matchedGroups.size() > 1)
            break;
    }

    return matchedGroups.size() > 1 ? std::stoi(matchedGroups[DURATION_GROUP].str()) : std::optional<int>{};
}

Uri WebViewParser::uri()
{
    auto mode = node().get<int>(ResourcesXlf::option(ResourcesXlf::WebView::ModeId), DEFAULT_WEBVIEW_MODE);

    if(mode != NATIVE_MODEID)
    {
        auto fileName = std::to_string(id()) + DEFAULT_WEBVIEW_EXTENSION;
        return Uri(DEFAULT_NATIVE_SCHEME + (Resources::resDirectory() / fileName).string());
    }
    else
    {
        auto uri = node().get<std::string>(ResourcesXlf::option(ResourcesXlf::Media::Uri));
        return Uri{removeEscapedSymbolsFromUri(uri)};
    }
}

std::string WebViewParser::removeEscapedSymbolsFromUri(std::string url)
{
    boost::replace_all(url, "%2F", "/");
    boost::replace_all(url, "%3A", ":");
    boost::replace_all(url, "%3F", "?");
    boost::replace_all(url, "%3D", "=");
    boost::replace_all(url, "%26", "&");

    return url;
}
