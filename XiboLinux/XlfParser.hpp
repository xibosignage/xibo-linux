#pragma once

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <spdlog/spdlog.h>

#include "MainLayout.hpp"
#include "Region.hpp"
#include "Image.hpp"
#include "XiboApp.hpp"

class Image;
class Video;
class WebView;

using ParsedImage = std::tuple<Image::ScaleType, Image::Align, Image::Valign>;
using ParsedVideo = std::tuple<bool, bool>;
using ParsedWebView = std::tuple<int, bool>;

class XlfParser
{
public:
    XlfParser(const std::string& full_path);
    std::unique_ptr<MainLayout> parse_layout();

private:
    void parse_xml_tree();
    void parse_region(const boost::property_tree::ptree& region_node);

    template<typename MediaType>
    void parse_media(const boost::property_tree::ptree& media_node);

    template<typename MediaType>
    typename std::enable_if<std::is_same<MediaType,Image>::value, ParsedImage>::type
    parse_media_options(const boost::property_tree::ptree& options);

    template<typename MediaType>
    typename std::enable_if<std::is_same<MediaType,Video>::value, ParsedVideo>::type
    parse_media_options(const boost::property_tree::ptree& options);

    template<typename MediaType>
    typename std::enable_if<std::is_same<MediaType,WebView>::value, ParsedWebView>::type
    parse_media_options(const boost::property_tree::ptree& options);

    template <typename MediaType, typename... Args, typename T, T... N>
    void add_media(int id, int duration, bool use_duration, const std::string& uri,
                   const std::tuple<Args...>& args, std::integer_sequence<T, N...>);

    std::string get_path(int id, const boost::optional<std::string>& uri, const std::string& type);

    Image::ScaleType from_scale_type(const std::string& option_name);
    Image::Align from_align(const std::string& option_name);
    Image::Valign from_valign(const std::string& option_name);

private:
    std::unique_ptr<MainLayout> m_layout;
    boost::property_tree::ptree m_tree;

};

template <typename MediaType, typename... Args, typename T, T... N>
void XlfParser::add_media(int id, int duration, bool use_duration, const std::string& uri,
                           const std::tuple<Args...>& args, std::integer_sequence<T, N...>)
{
    auto&& current_region = m_layout->region(m_layout->regions_count() - 1);
    current_region.add_media<MediaType>(id, duration, use_duration, uri, std::get<N>(args)...);
}

template<typename MediaType>
void XlfParser::parse_media(const boost::property_tree::ptree& media_node)
{
    spdlog::get(LOGGER)->debug("parse media");

    auto attrs = media_node.get_child("<xmlattr>");
    auto options = media_node.get_child("options");

    int id = attrs.template get<int>("id");
    int duration = attrs.template get<int>("duration");
    bool use_duration = attrs.template get<bool>("useDuration");
    auto optional_uri = options.get_optional<std::string>("uri");
    auto type = attrs.template get<std::string>("type");
    // FIXME temporary workaround
    std::string uri = XiboApp::example_dir() + "/" + get_path(id, optional_uri, type);
    spdlog::get(LOGGER)->debug(uri);

    auto media_options = parse_media_options<MediaType>(options);
    add_media<MediaType>(id, duration, use_duration, uri, media_options, std::make_index_sequence<std::tuple_size<decltype(media_options)>::value>{});
}

template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,Image>::value, ParsedImage>::type
XlfParser::parse_media_options(const boost::property_tree::ptree& options)
{
    auto scale_type = from_scale_type(options.get<std::string>("scaleType", "center"));
    auto align = from_align(options.get<std::string>("align", "center"));
    auto valign = from_valign(options.get<std::string>("valign", "middle"));

    return std::make_tuple(scale_type, align, valign);
}

template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,Video>::value, ParsedVideo>::type
XlfParser::parse_media_options(const boost::property_tree::ptree& options)
{
    bool muted = options.get<bool>("mute", false);
    bool looped = options.get<bool>("loop", false);

    return std::make_tuple(muted, looped);
}

template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,WebView>::value, ParsedWebView>::type
XlfParser::parse_media_options(const boost::property_tree::ptree& options)
{
    int mode_id = options.get_optional<int>("modeId").value_or(-1);
    bool transparency = options.get_optional<bool>("transparency").value_or(true);

    return std::make_tuple(mode_id, transparency);
}
