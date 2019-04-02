#pragma once

#include "constants.hpp"
#include "Options.hpp"

#include <boost/property_tree/ptree.hpp>

class MediaParser
{
public:
    MediaParser(const xml_node& node);
    virtual ~MediaParser() = default;

protected:
    const xml_node& node() const;
    MediaOptions baseOptions();

    virtual int id();
    virtual Uri uri();
    virtual int duration();

private:
    xml_node m_node;
    int m_id;
    boost::optional<std::string> m_uri;
    int m_duration;

};
