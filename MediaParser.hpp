#pragma once

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "constants.hpp"

struct ParsedMedia
{
    ParsedMedia(int _id, int _duration, const std::string& _uri) :
        id(_id), duration(_duration), uri(_uri) { }

    int id;
    int duration;
    std::string uri;
};

template <typename T>
class MediaParser
{
public:
    MediaParser(const boost::property_tree::ptree& tree) : m_tree(tree)
    {
        spdlog::get(LOGGER)->debug("parse media");

        auto attrs = m_tree.get_child("<xmlattr>");
        m_options = m_tree.get_child("options");

        m_id = attrs.template get<int>("id");
        m_duration = attrs.template get<int>("duration");
        m_uri = m_options.template get<std::string>("uri");
    }

protected:
    int m_id;
    int m_duration;
    std::string m_uri;
    boost::property_tree::ptree m_options;

private:
    boost::property_tree::ptree m_tree;

};
