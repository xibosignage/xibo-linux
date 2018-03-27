#pragma once

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <spdlog/spdlog.h>

#include "XiboApp.hpp"
#include "constants.hpp"

class Image;
class Video;
class WebView;

using Params = boost::property_tree::ptree;

template<typename First, typename Second>
using is_same_media = std::enable_if_t<std::is_same<First,Second>::value>;

class XlfParser
{
public:
    XlfParser(const std::string& full_path);
    void parse_xlf_tree();
    boost::property_tree::ptree get_layout_params() const;

private:
    int parse_region_params(const boost::property_tree::ptree& region_node);
    void parse_layout_params(const boost::property_tree::ptree& layout_node);

    template<typename MediaType>
    void parse_media_params(int region_id, const boost::property_tree::ptree& media_node);

    template<typename MediaType>
    void parse_media_options(Params& media_params, const boost::property_tree::ptree& options, is_same_media<MediaType,Image>* dummy = 0);

    template<typename MediaType>
    void parse_media_options(Params& media_params, const boost::property_tree::ptree& options, is_same_media<MediaType,Video>* dummy = 0);

    template<typename MediaType>
    void parse_media_options(Params& media_params, const boost::property_tree::ptree& options, is_same_media<MediaType,WebView>* dummy = 0);

    std::string get_path(int id, const boost::optional<std::string>& uri, const std::string& type);
    boost::optional<int> parse_duration(const std::string& path);

private:
    boost::property_tree::ptree m_tree;
    boost::property_tree::ptree m_params;
};

template<typename MediaType>
void XlfParser::parse_media_params(int region_id, const boost::property_tree::ptree& media_node)
{
    spdlog::get(LOGGER)->debug("parse media");

    auto attrs = media_node.get_child("<xmlattr>");
    auto options = media_node.get_child("options");
    auto& medias = m_params.get_child("regions").get_child(std::to_string(region_id)).get_child("medias");
    int id = attrs.template get<int>("id");

    auto& media_params = medias.add_child(std::to_string(id), boost::property_tree::ptree{});

    auto optional_uri = options.get_optional<std::string>("uri");
    std::string uri = XiboApp::example_dir() + "/" + get_path(id, optional_uri, attrs.template get<std::string>("type")); // FIXME temporary workaround

    media_params.put("id", id);
    media_params.put("duration", parse_duration(uri).value_or(attrs.get<int>("duration")));
    media_params.put("uri", uri);

    parse_media_options<MediaType>(media_params, options);
}

template<typename MediaType>
void XlfParser::parse_media_options(Params& media_params, const boost::property_tree::ptree& options, is_same_media<MediaType,Image>*)
{
    media_params.put("type", "image"); // FIXME change to enum
    media_params.put("scaleType", options.get<std::string>("scaleType", "center"));
    media_params.put("align", options.get<std::string>("align", "center"));
    media_params.put("valign", options.get<std::string>("valign", "middle"));
}

template<typename MediaType>
void XlfParser::parse_media_options(Params& media_params, const boost::property_tree::ptree& options, is_same_media<MediaType,Video>*)
{
    media_params.put("type", "video");
    media_params.put("mute", options.get<bool>("mute", false));
    media_params.put("loop", options.get<bool>("loop", false));
}

template<typename MediaType>
void XlfParser::parse_media_options(Params& media_params, const boost::property_tree::ptree& options, is_same_media<MediaType,WebView>*)
{
    media_params.put("type", "webview");
    media_params.put("modeId", options.get_optional<int>("modeId").value_or(-1));
    media_params.put("transparency", options.get_optional<bool>("transparency").value_or(true));
}
