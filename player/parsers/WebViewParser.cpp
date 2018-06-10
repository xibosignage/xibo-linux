#include "WebViewParser.hpp"
#include "media/WebView.hpp"
#include "utils/utilities.hpp"

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <regex>

WebViewParser::WebViewParser(const xlf_node& parent_node, const xlf_node& media_node) :
    MediaParser(parent_node, media_node)
{
}

std::unique_ptr<Media> WebViewParser::parse()
{
    int id = attrs().template get<int>("id");
    std::string uri = get_path(id, options().get_optional<std::string>("uri"));
    int duration = parse_duration(uri).value_or(attrs().get<int>("duration"));
    int width = parent_node().get_child("<xmlattr>").get<double>("width");
    int height = parent_node().get_child("<xmlattr>").get<double>("height");

    int mode_id = options().get<int>("modeId", -1);
    bool transparency = options().get<bool>("transparency", true);

    auto webview = std::make_unique<WebView>(id, Size{width, height}, duration, uri, mode_id, transparency);
    webview->attach_audio(parse_audio_node());
    return webview;
}

boost::optional<int> WebViewParser::parse_duration(const std::string& path)
{
    auto extension = boost::filesystem::extension(path);
    std::ifstream in(path);
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

// FIXME temporary workaround
std::string WebViewParser::get_path(int id, const boost::optional<std::string>& uri)
{
    auto folder = utils::example_dir() + "/";
    if(!uri || !boost::filesystem::exists(folder + uri.value()))
    {
        if(boost::filesystem::exists(folder + "requiredFiles.xml"))
        {
            boost::property_tree::ptree tree;
            boost::property_tree::read_xml(folder + "requiredFiles.xml", tree);

            auto required_files = tree.get_child("RequiredFiles").get_child("RequiredFileList");
            for(auto&& required_file : required_files)
            {
                auto file_info = required_file.second;
                if(file_info.get<std::string>("FileType") == "resource" && file_info.get<int>("MediaId") == id)
                {
                    return folder + file_info.get<std::string>("Path");
                }
            }
        }
        return std::string{};
    }
    return folder + uri.value();
}
