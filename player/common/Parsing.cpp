#include "Parsing.hpp"
#include "fs/FilePath.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

ptree_node Parsing::xmlFromPath(const FilePath& xlfPath)
{
    ptree_node tree;
    boost::property_tree::read_xml(xlfPath, tree);
    return tree;
}

ptree_node Parsing::xmlFromString(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    ptree_node tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}

ptree_node Parsing::jsonFromString(const std::string& json)
{
    std::stringstream stream;
    stream << json;
    ptree_node tree;
    boost::property_tree::read_json(stream, tree);
    return tree;
}

std::string Parsing::xmlTreeToString(const ptree_node& node)
{
    std::stringstream sstream;
    boost::property_tree::write_xml(sstream, node);
    return sstream.str();
}

std::string Parsing::xmlTreeToEscapedString(const ptree_node& node)
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
