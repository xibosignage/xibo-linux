#pragma once

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <spdlog/spdlog.h>

#include "../XiboApp.hpp"
#include "constants.hpp"

using xlf_node = boost::property_tree::ptree;

class XlfParser
{
public:
    XlfParser(const std::string& full_path);

    Params parse_layout();
    Params parse_region(const xlf_node& region_node);
    Params parse_media(const xlf_node& media_node);

private:
    Params parse_layout_params(const xlf_node& layout_node);
    Params parse_region_params(const xlf_node& region_node);
    template<MediaType type> Params parse_media_params(const xlf_node& media_node);
    template<MediaType type> void parse_media_options(Params& media_params, const xlf_node& options);

    std::string get_path(int id, const boost::optional<std::string>& uri, const std::string& type);
    boost::optional<int> parse_duration(const std::string& path);

private:
    boost::property_tree::ptree m_tree;
    Params m_params;

};

template<MediaType type> Params XlfParser::parse_media_params(const xlf_node& media_node)
{
    spdlog::get(LOGGER)->debug("parse media");
    xlf_node media_params;

    auto attrs = media_node.get_child("<xmlattr>");
    auto options = media_node.get_child("options");
    int id = attrs.template get<int>("id");

    auto optional_uri = options.get_optional<std::string>("uri");
    std::string uri = XiboApp::example_dir() + "/" + get_path(id, optional_uri, attrs.template get<std::string>("type")); // FIXME temporary workaround
    media_params.put("uri", uri);
    media_params.put("id", id);

    if constexpr(type == MediaType::WebView)
        media_params.put("duration", parse_duration(uri).value_or(attrs.get<int>("duration")));
    else
        media_params.put("duration", attrs.get<int>("duration"));

    parse_media_options<type>(media_params, options);

    return media_params;
}

template<MediaType type> void XlfParser::parse_media_options(Params& media_params, const xlf_node& options)
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
