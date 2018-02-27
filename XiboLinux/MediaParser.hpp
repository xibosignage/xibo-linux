#pragma once

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Image.hpp"

using ParsedImage = std::tuple<uint, uint, bool, std::string, Image::ScaleType, Image::Align, Image::Valign>;
using ParsedVideo = std::tuple<uint, uint, bool, std::string, bool, bool>;
using ParsedWebView = std::tuple<uint, uint, bool, std::string, int, bool>;

class Image;
class Video;
class WebView;

class MediaParser
{
public:
    MediaParser(const boost::property_tree::ptree& tree);

    template<typename MediaType>
    typename std::enable_if<std::is_same<MediaType,Image>::value, ParsedImage>::type parse_media();

    template<typename MediaType>
    typename std::enable_if<std::is_same<MediaType,Video>::value, ParsedVideo>::type parse_media();

    template<typename MediaType>
    typename std::enable_if<std::is_same<MediaType,WebView>::value, ParsedWebView>::type parse_media();

private:
    std::string get_path(const std::string& type);

    Image::ScaleType from_scale_type(const std::string& option_name);
    Image::Align from_align(const std::string& option_name);
    Image::Valign from_valign(const std::string& option_name);

protected:
    int m_id;
    int m_duration;
    bool m_use_duration;
    std::string m_uri;
    boost::property_tree::ptree m_options;

private:
    boost::property_tree::ptree m_tree;

};

template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,Image>::value, ParsedImage>::type MediaParser::parse_media()
{
    auto scale_type = from_scale_type(m_options.get<std::string>("scaleType", "center"));
    auto align = from_align(m_options.get<std::string>("align", "center"));
    auto valign = from_valign(m_options.get<std::string>("valign", "middle"));

    return std::make_tuple(m_id, m_duration, m_use_duration, m_uri, scale_type, align, valign);
}

template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,Video>::value, ParsedVideo>::type MediaParser::parse_media()
{
    bool muted = m_options.get<bool>("mute", false);
    bool looped = m_options.get<bool>("loop", false);

    return std::make_tuple(m_id, m_duration, m_use_duration, m_uri, muted, looped);
}

template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,WebView>::value, ParsedWebView>::type MediaParser::parse_media()
{
    int mode_id = m_options.get_optional<int>("modeId").value_or(-1);
    bool transparency = m_options.get_optional<bool>("transparency").value_or(true);

    return std::make_tuple(m_id, m_duration, m_use_duration, m_uri, mode_id, transparency);
}
