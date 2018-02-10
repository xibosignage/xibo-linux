#pragma once

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "constants.hpp"

template <typename T>
class MediaParser
{
public:
    MediaParser(const boost::property_tree::ptree& tree);

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

#include <boost/program_options.hpp>
extern boost::program_options::variables_map vm;

template<typename T>
MediaParser<T>::MediaParser(const boost::property_tree::ptree& tree) : m_tree(tree)
{
    spdlog::get(LOGGER)->debug("parse media");

    auto attrs = m_tree.get_child("<xmlattr>");
    m_options = m_tree.get_child("options");

    m_id = attrs.template get<int>("id");
    m_duration = attrs.template get<int>("duration");
    m_use_duration = attrs.template get<bool>("useDuration");
    m_uri = vm["example-dir"].as<std::string>() + "/" + get_path(attrs.template get<std::string>("type"));
    spdlog::get(LOGGER)->debug(m_uri);
}

template<typename T>
std::string MediaParser<T>::get_path(const std::string& type)
{
    auto uri = m_options.get_optional<std::string>("uri");
    if(!uri || type == "ticker" || type == "forecastio")
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(vm["example-dir"].as<std::string>() + "/requiredFiles.xml", tree);

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
