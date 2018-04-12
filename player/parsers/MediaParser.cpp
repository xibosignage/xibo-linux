#include "MediaParser.hpp"

#include <regex>
#include <fstream>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem/convenience.hpp>

MediaParser::MediaParser(const xlf_node& media_node) :
    m_media_node(media_node)
{

}

AnyMedia MediaParser::parse_media()
{
    auto media_type = to_media_type(m_media_node.get_child("<xmlattr>").get<std::string>("type"));
    AnyMedia media;

    switch(media_type)
    {
    case MediaType::Image:
        media = parse_media_params<MediaType::Image>(m_media_node);
        break;
    case MediaType::Video:
        media = parse_media_params<MediaType::Video>(m_media_node);
        break;
    case MediaType::Audio:
        media = parse_media_params<MediaType::Audio>(m_media_node);
        break;
    case MediaType::WebView:
        media = parse_media_params<MediaType::WebView>(m_media_node);
        break;
    }
    media.media_type = media_type;

    return media;
}

template<MediaType type> AnyMedia MediaParser::parse_media_params(const xlf_node& media_node)
{
    spdlog::get(LOGGER)->debug("parse media");

    ParsedMedia<type> media_params;
    auto attrs = media_node.get_child("<xmlattr>");
    auto options = media_node.get_child("options");
    auto audio_node = media_node.get_child_optional("audio");

    int id = attrs.template get<int>("id");
    std::string uri = get_path(id, options.get_optional<std::string>("uri"));

    media_params.id = id;
    media_params.uri = uri;
    media_params.duration = parse_duration(uri).value_or(attrs.get<int>("duration"));

    parse_media_options<type>(media_params, options);

    if(audio_node)
    {
        auto audio = parse_audio_node(audio_node.value());
        audio->duration = media_params.duration;
        media_params.audio = audio;
    }

    return media_params;
}

template<MediaType type> void MediaParser::parse_media_options(ParsedMedia<type>& media_params, const xlf_node& options)
{
    if constexpr(type == MediaType::Image)
    {
        media_params.scale_type = options.get_optional<std::string>("scaleType");
        media_params.align = options.get_optional<std::string>("align");
        media_params.valign = options.get_optional<std::string>("valign");
    }
    else if constexpr(type == MediaType::Video || type == MediaType::Audio)
    {
        media_params.mute = options.get_optional<bool>("mute");
        media_params.loop = options.get_optional<bool>("loop");

        if constexpr(type == MediaType::Audio)
        {
            media_params.volume = options.get_optional<int>("volume");
        }
    }
    else if constexpr(type == MediaType::WebView)
    {
        media_params.mode_id = options.get_optional<int>("modeId");
        media_params.transparency = options.get_optional<bool>("transparency");
    }
}

boost::optional<int> MediaParser::parse_duration(const std::string& path)
{
    auto extension = boost::filesystem::extension(path);
    if(extension == ".html" || extension == ".htm")
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
    }
    return boost::optional<int>{};
}

// FIXME temporary workaround
std::string MediaParser::get_path(int id, const boost::optional<std::string>& uri)
{
    auto folder = utilities::example_dir() + "/";
    if(!uri || !boost::filesystem::exists(folder + uri.value()))
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
        return std::string{};
    }
    return folder + uri.value();
}

std::shared_ptr<ParsedMedia<MediaType::Audio>> MediaParser::parse_audio_node(const xlf_node& audio_node)
{
    auto uri_node = audio_node.get_child("uri");
    auto attrs = uri_node.get_child("<xmlattr>");

    auto audio = std::make_shared<ParsedMedia<MediaType::Audio>>();
    audio->id = attrs.get<int>("mediaId");
    audio->uri = utilities::example_dir() + "/" + uri_node.get_value<std::string>();
    audio->mute = attrs.get_optional<bool>("mute");
    audio->loop = attrs.get_optional<bool>("loop");
    audio->volume = attrs.get_optional<int>("volume");
    return audio;
}

MediaType MediaParser::to_media_type(const std::string& type)
{
    if(type == "image")
        return MediaType::Image;
    else if(type == "video")
        return MediaType::Video;
    else if(type == "audio")
        return MediaType::Audio;
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        return MediaType::WebView;
}
