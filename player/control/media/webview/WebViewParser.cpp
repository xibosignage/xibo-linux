#include "WebViewParser.hpp"

#include "common/fs/Resources.hpp"
#include "utils/Managers.hpp"

#include "networking/WebServer.hpp" // FIXME remove dependency

#include "control/media/MediaResources.hpp"
#include "control/media/webview/WebViewResources.hpp"

#include <regex>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>

const bool DefaultTransparency = true;
const int DefaultWebviewMode = 0;
const int NativeModeid = 1;

const std::regex DurationRegex("DURATION=([0-9]+)");
const std::string DefaultWebviewExtension = ".html";

int WebViewParser::durationFrom(const ptree_node& node)
{
    auto baseDuration = MediaParser::durationFrom(node);
    return parseDuration(uriFrom(node).path()).value_or(baseDuration);
}

ExtraOptions WebViewParser::extraOptionsImpl(const ptree_node& node)
{
    auto transparency = node.get<bool>(XlfResources::WebView::Transparency, DefaultTransparency);
    auto mode = node.get<int>(XlfResources::WebView::ModeId, DefaultWebviewMode);

    return {
        {XlfResources::WebView::Transparency, std::to_string(transparency)},
        {XlfResources::WebView::ModeId, std::to_string(mode)}
    };
}

std::optional<int> WebViewParser::parseDuration(const FilePath& path)
{
    std::ifstream in(path.string());

    std::smatch matchedGroups;
    std::string line;
    const int DurationGroup = 1;

    while(std::getline(in, line))
    {
        if(std::regex_search(line, matchedGroups, DurationRegex) && matchedGroups.size() > 1)
            break;
    }

    return matchedGroups.size() > 1 ? std::stoi(matchedGroups[DurationGroup].str()) : std::optional<int>{};
}

Uri WebViewParser::uriFrom(const ptree_node& node)
{
    auto mode = node.get<int>(XlfResources::WebView::ModeId, DefaultWebviewMode);

    if(mode != NativeModeid)
    {
        auto fileName = std::to_string(idFrom(node)) + DefaultWebviewExtension;
        return Uri{Managers::webserver().address() + fileName};
    }
    else
    {
        auto uri = node.get<std::string>(XlfResources::Media::Uri);
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
