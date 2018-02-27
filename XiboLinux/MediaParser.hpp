#pragma once

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "constants.hpp"
#include "XiboApp.hpp"
#include "Image.hpp"

using ParsedImage = std::tuple<uint, uint, bool, std::string, Image::ScaleType, Image::Align, Image::Valign>;
using ParsedVideo = std::tuple<uint, uint, bool, std::string, bool, bool>;
using ParsedWebView = std::tuple<uint, uint, bool, std::string, int, bool>;

class Image;
class Video;
class WebView;

template<typename T>
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

protected:
    int m_id;
    int m_duration;
    bool m_use_duration;
    std::string m_uri;
    boost::property_tree::ptree m_options;

private:
    boost::property_tree::ptree m_tree;

};

template<typename T>
template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,Image>::value, ParsedImage>::type MediaParser<T>::parse_media()
{

}

template<typename T>
template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,Video>::value, ParsedVideo>::type MediaParser<T>::parse_media()
{

}

template<typename T>
template<typename MediaType>
typename std::enable_if<std::is_same<MediaType,WebView>::value, ParsedWebView>::type MediaParser<T>::parse_media()
{

}


template<typename T>
MediaParser<T>::MediaParser(const boost::property_tree::ptree& tree) : m_tree(tree)
{
    spdlog::get(LOGGER)->debug("parse media");

    auto attrs = m_tree.get_child("<xmlattr>");
    m_options = m_tree.get_child("options");

    m_id = attrs.template get<int>("id");
    m_duration = attrs.template get<int>("duration");
    m_use_duration = attrs.template get<bool>("useDuration");
    m_uri = XiboApp::example_dir() + "/" + get_path(attrs.template get<std::string>("type"));
    spdlog::get(LOGGER)->debug(m_uri);
}


// FIXME
template<typename T>
std::string MediaParser<T>::get_path(const std::string& type)
{
    auto uri = m_options.get_optional<std::string>("uri");
    if(!uri || type == "ticker" || type == "forecastio")
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(XiboApp::example_dir() + "/requiredFiles.xml", tree);

        auto required_files = tree.get_child("RequiredFiles").get_child("RequiredFileList");
        for(auto&& required_file : required_files)
        {
            auto file_info = required_file.second;
            if(file_info.get<std::string>("FileType") == "resource" && file_info.get<int>("MediaId") == m_id)
            {
                return file_info.get<std::string>("Path");
            }
        }
        return std::string{};
    }
    return uri.value();
}
