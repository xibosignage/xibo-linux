#pragma once

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "constants.hpp"

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
        m_use_duration = attrs.template get<bool>("useDuration");
        m_uri = m_options.template get<std::string>("uri");
    }

protected:
    int m_id;
    int m_duration;
    bool m_use_duration;
    std::string m_uri;
    boost::property_tree::ptree m_options;

private:
    boost::property_tree::ptree m_tree;

};
