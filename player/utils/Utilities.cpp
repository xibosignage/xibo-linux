#include "Utilities.hpp"
#include "constants.hpp"
#include "XiboApp.hpp"

#include <boost/property_tree/xml_parser.hpp>

std::shared_ptr<spdlog::logger> Utils::logger()
{
    static auto logger = spdlog::get(LOGGER);
    return logger;
}

DownloadManager& Utils::downloadManager()
{
    return XiboApp::app().downloadManager();
}

XMDSManager& Utils::xmdsManager()
{
    return XiboApp::app().xmdsManager();
}

boost::property_tree::ptree Utils::parseXmlFromPath(const std::filesystem::path& xlfPath)
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
