#include "WebViewParser.hpp"

#include "XiboApp.hpp"
#include "control/media/Media.hpp"
#include "control/media/MediaResources.hpp"
#include "control/media/webview/WebViewFactory.hpp"
#include "control/media/webview/WebViewResources.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/fs/Resource.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <regex>

const bool DefaultTransparency = true;
const int DefaultWebviewMode = 0;
const int NativeModeid = 1;

const std::regex DurationRegex("DURATION=([0-9]+)");
const std::regex ViewPortWidth{"(content=\"width=)(.*)(\".*)"};
const std::string DefaultWebviewExtension = ".html";

int WebViewParser::durationFrom(const XmlNode& node)
{
    auto baseDuration = MediaParser::durationFrom(node);
    return parseDuration(uriFrom(node).path()).value_or(baseDuration);
}

boost::optional<int> WebViewParser::parseDuration(const FilePath& path)
{
    auto fileContent = FileSystem::readFromFile(path);

    std::smatch matchedGroups;
    const int DurationGroup = 1;

    std::regex_search(fileContent, matchedGroups, DurationRegex);

    return matchedGroups.size() > 1 ? std::stoi(matchedGroups[DurationGroup].str()) : boost::optional<int>{};
}

Uri WebViewParser::uriFrom(const XmlNode& node)
{
    auto mode = node.get<int>(XlfResources::WebView::ModeId, DefaultWebviewMode);

    if (mode != NativeModeid)
    {
        std::string fileName{std::to_string(idFrom(node)) + DefaultWebviewExtension};
        return Uri::fromString(XiboApp::localAddress().string() + fileName);
    }
    else
    {
        auto uri = node.get<std::string>(XlfResources::Media::Uri);
        return Uri::fromString(removeEscapedSymbolsFromUri(uri));
    }
}

std::string WebViewParser::removeEscapedSymbolsFromUri(std::string url)
{
    boost::replace_all(url, "%21", "!");
    boost::replace_all(url, "%23", "#");
    boost::replace_all(url, "%24", "$");
    boost::replace_all(url, "%25", "%");
    boost::replace_all(url, "%26", "&");
    boost::replace_all(url, "%27", "'");
    boost::replace_all(url, "%28", "(");
    boost::replace_all(url, "%29", ")");
    boost::replace_all(url, "%2A", "*");
    boost::replace_all(url, "%2B", "+");
    boost::replace_all(url, "%2C", ",");
    boost::replace_all(url, "%2F", "/");
    boost::replace_all(url, "%3A", ":");
    boost::replace_all(url, "%3B", ";");
    boost::replace_all(url, "%3D", "=");
    boost::replace_all(url, "%3F", "?");
    boost::replace_all(url, "%40", "@");
    boost::replace_all(url, "%5B", "]");
    boost::replace_all(url, "%5D", "[");

    return url;
}

std::unique_ptr<Xibo::Media> WebViewParser::createMedia(const MediaOptions& options,
                                                        const XmlNode& node,
                                                        int width,
                                                        int height)
{
    auto transparency = node.get<bool>(XlfResources::WebView::Transparency, DefaultTransparency);

    WebViewFactory factory;
    return factory.create(options, width, height, transparency);
}
