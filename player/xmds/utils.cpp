#include "utils.hpp"
#include <boost/property_tree/xml_parser.hpp>

boost::property_tree::ptree utils::parse_xml(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}
