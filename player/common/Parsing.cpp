#include "Parsing.hpp"
#include "fs/FilePath.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

PtreeNode Parsing::xmlFromPath(const FilePath& xlfPath)
{
    PtreeNode tree;
    boost::property_tree::read_xml(xlfPath, tree);
    return tree;
}

PtreeNode Parsing::xmlFromString(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    PtreeNode tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}

PtreeNode Parsing::jsonFromString(const std::string& json)
{
    std::stringstream stream;
    stream << json;
    PtreeNode tree;
    boost::property_tree::read_json(stream, tree);
    return tree;
}

std::string Parsing::xmlTreeToString(const PtreeNode& node)
{
    std::stringstream sstream;
    boost::property_tree::write_xml(sstream, node);
    return sstream.str();
}

std::string Parsing::xmlTreeToEscapedString(const PtreeNode& node)
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
