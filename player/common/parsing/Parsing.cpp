#include "Parsing.hpp"

#include "common/fs/FilePath.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

const auto XmlFormatSettings = boost::property_tree::xml_writer_make_settings<std::string>(' ', 4);

XmlNode Parsing::xmlFrom(const FilePath& xlfPath)
{
    XmlNode tree;
    boost::property_tree::read_xml(xlfPath.string(), tree);
    return tree;
}

XmlNode Parsing::xmlFrom(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    XmlNode tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}

JsonNode Parsing::jsonFromString(const std::string& json)
{
    std::stringstream stream;
    stream << json;
    JsonNode tree;
    boost::property_tree::read_json(stream, tree);
    return tree;
}

std::string Parsing::jsonToString(const JsonNode& tree)
{
    std::stringstream sstream;
    boost::property_tree::write_json(sstream, tree);
    return sstream.str();
}

void Parsing::xmlTreeToFile(const FilePath& path, const XmlNode& node)
{
    boost::property_tree::write_xml(path.string(), node, std::locale{}, XmlFormatSettings);
}

std::string Parsing::xmlTreeToString(const XmlNode& node)
{
    std::stringstream sstream;
    boost::property_tree::write_xml(sstream, node, XmlFormatSettings);
    return sstream.str();
}

std::string Parsing::xmlTreeToEscapedString(const XmlNode& node)
{
    std::string xmlStr = xmlTreeToString(node);

    boost::replace_all(xmlStr, "<", "&lt;");
    boost::replace_all(xmlStr, ">", "&gt;");
    boost::replace_all(xmlStr, "&", "&amp;");
    boost::replace_all(xmlStr, "'", "&apos;");
    boost::replace_all(xmlStr, "\"", "&quot;");

    return xmlStr;
}

std::string Parsing::xmlAttr(const std::string& property)
{
    return "<xmlattr>." + property;
}

std::string Parsing::xmlOption(const std::string& property)
{
    return "options." + property;
}
