#include "WebViewParser.hpp"
#include "media/WebView.hpp"
#include "utils/utilities.hpp"

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <regex>

const std::string DEFAULT_EXTENSION = ".html";

WebViewParser::WebViewParser(const xlf_node& parent_node, const xlf_node& media_node) :
    MediaParser(parent_node, media_node)
{
}

std::unique_ptr<Media> WebViewParser::doParse()
{
    int id = attrs().template get<int>("id");
    auto filename = std::to_string(id) + DEFAULT_EXTENSION;
    auto uri = utils::resources_dir() / filename;
    int duration = parse_duration(uri).value_or(attrs().get<int>("duration"));
    int width = parent_node().get_child("<xmlattr>").get<double>("width");
    int height = parent_node().get_child("<xmlattr>").get<double>("height");

    int mode_id = options().get<int>("modeId", -1);
    bool transparency = options().get<bool>("transparency", true);

    return std::make_unique<WebView>(id, width, height, duration, uri.string(), mode_id, transparency);
}

boost::optional<int> WebViewParser::parse_duration(const boost::filesystem::path& path)
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
