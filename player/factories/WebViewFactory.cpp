#include "WebViewFactory.hpp"
#include "media/WebView.hpp"
#include "media/IMedia.hpp"
#include "utils/utilities.hpp"

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <regex>

const std::string DEFAULT_EXTENSION = ".html";

WebViewFactory::WebViewFactory(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaFactory(parentNode, mediaNode)
{
}

std::unique_ptr<IMedia> WebViewFactory::doCreate()
{
    int id = attrs().template get<int>("id");
    auto filename = std::to_string(id) + DEFAULT_EXTENSION;
    auto uri = utils::resourcesDir() / filename;
    int duration = parseDuration(uri).value_or(attrs().get<int>("duration"));
    int width = parentNode().get_child("<xmlattr>").get<double>("width");
    int height = parentNode().get_child("<xmlattr>").get<double>("height");

    int modeId = options().get<int>("modeId", -1);
    bool transparency = options().get<bool>("transparency", true);

    return std::make_unique<WebView>(id, width, height, duration, uri.string(), modeId, transparency);
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
