#include "WebViewFactory.hpp"

#include "media/WebView.hpp"
#include "media/IMedia.hpp"
#include "utils/Resources.hpp"

#include "adaptors/WebKitWebViewAdaptor.hpp"

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <regex>

const std::string DEFAULT_EXTENSION = ".htm";

WebViewFactory::WebViewFactory(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaFactory(parentNode, mediaNode)
{
}

std::unique_ptr<IMedia> WebViewFactory::doCreate()
{
    int id = attrs().template get<int>("id");
    auto filename = std::to_string(id) + DEFAULT_EXTENSION;
    auto uri = Resources::directory() / filename;
    int duration = parseDuration(uri).value_or(attrs().get<int>("duration"));
    int width = static_cast<int>(parentNode().get_child("<xmlattr>").get<double>("width"));
    int height = static_cast<int>(parentNode().get_child("<xmlattr>").get<double>("height"));

//    int modeId = options().get<int>("modeId", -1);
    bool transparency = options().get<bool>("transparency", true);

    auto adaptor = std::make_unique<WebKitWebViewAdaptor>();
    adaptor->setSize(width, height);
    if(transparency)
    {
        adaptor->enableTransparency();
    }
    adaptor->load(uri);

    auto webview = std::make_unique<WebView>(std::move(adaptor));
    webview->setDuration(duration);
    return webview;
}

boost::optional<int> WebViewFactory::parseDuration(const std::filesystem::path& path)
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
    return boost::optional<int>{};
}
