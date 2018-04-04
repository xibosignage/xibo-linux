#pragma once

#include "constants.hpp"
#include "utils/utilities.hpp"

#include <spdlog/spdlog.h>

class MediaParser
{
public:
    MediaParser() = default;
    Params parse_media(int region_id, int media_id);

private:
    template<MediaType type> Params parse_media_params(const xlf_node& media_node);
    template<MediaType type> void parse_media_options(Params& media_params, const xlf_node& options);
    void parse_audio_options(Params& media_params, const xlf_node& options);

    std::string get_path(int id, const boost::optional<std::string>& uri, const std::string& type);
    boost::optional<int> parse_duration(const std::string& path);

};

template<MediaType type> Params MediaParser::parse_media_params(const xlf_node& media_node)
{
    spdlog::get(LOGGER)->debug("parse media");
    Params media_params;

    auto attrs = media_node.get_child("<xmlattr>");
    auto options = media_node.get_child("options");
    int id = attrs.template get<int>("id");

    auto optional_uri = options.get_optional<std::string>("uri");
    std::string uri = utilities::example_dir() + "/" + get_path(id, optional_uri, attrs.template get<std::string>("type")); // FIXME temporary workaround
    media_params.put("uri", uri);
    media_params.put("id", id);

    if constexpr(type == MediaType::WebView)
        media_params.put("duration", parse_duration(uri).value_or(attrs.get<int>("duration")));
    else
        media_params.put("duration", attrs.get<int>("duration"));

    auto audio_options = media_node.get_child_optional("audio");
    if(audio_options)
        parse_audio_options(media_params, audio_options.value());

    parse_media_options<type>(media_params, options);

    return media_params;
}

template<MediaType type> void MediaParser::parse_media_options(Params& media_params, const xlf_node& options)
{
    media_params.put("type", type);
    if constexpr(type == MediaType::Image)
    {
        media_params.put("scaleType", options.get<std::string>("scaleType", "center"));
        media_params.put("align", options.get<std::string>("align", "center"));
        media_params.put("valign", options.get<std::string>("valign", "middle"));
    }
    else if constexpr(type == MediaType::Video || type == MediaType::Audio)
    {
        media_params.put("mute", options.get<bool>("mute", false));
        media_params.put("loop", options.get<bool>("loop", false));
    }
    else if constexpr(type == MediaType::WebView)
    {
        media_params.put("modeId", options.get_optional<int>("modeId").value_or(-1));
        media_params.put("transparency", options.get_optional<bool>("transparency").value_or(true));
    }
}
