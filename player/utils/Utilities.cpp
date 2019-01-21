#include "Utilities.hpp"
#include "XiboApp.hpp"
#include "FilePath.hpp"

#include <boost/property_tree/xml_parser.hpp>

HTTPManager& Utils::httpManager()
{
    return XiboApp::app().downloadManager();
}

XMDSManager& Utils::xmdsManager()
{
    return XiboApp::app().xmdsManager();
}

boost::property_tree::ptree Utils::parseXmlFromPath(const FilePath& xlfPath)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(xlfPath.string(), tree);
    return tree;
}

boost::property_tree::ptree Utils::parseXmlFromString(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}
