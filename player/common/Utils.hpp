#pragma once

#include <boost/property_tree/ptree.hpp>

using xml_node = boost::property_tree::ptree;

class FilePath;

namespace Utils
{
    xml_node parseXmlFromPath(const FilePath& xlfPath);
    xml_node parseXmlFromString(const std::string& xml);
    xml_node parseJsonFromString(const std::string& json);
    std::string xmlTreeToEscapedString(const xml_node& node);
    std::string md5hash(std::string_view data);
    std::string toBase64(const std::string& text);
    std::string fromBase64(const std::string& text);
}
