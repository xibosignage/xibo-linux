#include "MediaParser.hpp"
#include "parsers/XlfParser.hpp"

#include <regex>
#include <fstream>
#include <boost/property_tree/xml_parser.hpp>

Params MediaParser::parse_media(int region_id, int media_id)
{
    auto media_node = utilities::xlf_parser().media_node(region_id, media_id);
    auto type = media_node.get_child("<xmlattr>").get<std::string>("type");
    auto render = media_node.get_child("<xmlattr>").get<std::string>("render");

    if(type == "image")
        return parse_media_params<MediaType::Image>(media_node);
    else if(type == "video")
        return parse_media_params<MediaType::Video>(media_node);
    else if(type == "audio")
        return parse_media_params<MediaType::Audio>(media_node);
    // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
    else if(render == "html" || type == "text" || type == "ticker" || type == "datasetview" || type == "embedded")
        return parse_media_params<MediaType::WebView>(media_node);

    return {};
}

// FIXME temporary workaround
std::string MediaParser::get_path(int id, const boost::optional<std::string>& uri, const std::string& type)
{
    if(!uri || type == "ticker" || type == "forecastio")
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(utilities::example_dir() + "/requiredFiles.xml", tree);

        auto required_files = tree.get_child("RequiredFiles").get_child("RequiredFileList");
        for(auto&& required_file : required_files)
        {
            auto file_info = required_file.second;
            if(file_info.get<std::string>("FileType") == "resource" && file_info.get<int>("MediaId") == id)
            {
                return file_info.get<std::string>("Path");
            }
        }
        return std::string{};
    }
    return uri.value();
}

boost::optional<int> MediaParser::parse_duration(const std::string& path)
{
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

void MediaParser::parse_audio_options(Params& media_params, const xlf_node& options)
{
    spdlog::get(LOGGER)->debug("parse audio options");

    auto uri_node = options.get_child("uri");
    auto attrs = uri_node.get_child("<xmlattr>");
    Params audio_params;
    audio_params.put("type", MediaType::Audio);
    audio_params.put("duration", media_params.get<int>("duration"));
    audio_params.put("id", attrs.get<int>("mediaId"));
    audio_params.put("loop", attrs.get<bool>("loop", false));
    audio_params.put("mute", attrs.get<bool>("mute", false));
    audio_params.put("volume", attrs.get<int>("volume", 100));
    audio_params.put("uri", utilities::example_dir() + "/" + uri_node.get_value<std::string>());
    media_params.add_child("audio", audio_params);
}
