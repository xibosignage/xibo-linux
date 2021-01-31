#pragma once

#include <boost/property_tree/ptree.hpp>

class FilePath;
using XmlNode = boost::property_tree::ptree;
using JsonNode = boost::property_tree::ptree;
using NodePath = boost::property_tree::path;

namespace Parsing
{
    XmlNode xmlFrom(const FilePath& xlfPath);
    XmlNode xmlFrom(const std::string& xml);
    JsonNode jsonFromString(const std::string& json);
    std::string jsonToString(const JsonNode& tree);
    std::string xmlTreeToString(const XmlNode& node);
    void xmlTreeToFile(const FilePath& path, const XmlNode& node);
    std::string xmlTreeToEscapedString(const XmlNode& node);
    std::string xmlAttr(const std::string& property);
    std::string xmlOption(const std::string& property);
}
